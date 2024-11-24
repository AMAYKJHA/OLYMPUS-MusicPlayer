
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <SDL_mixer.h>
#include <iostream>
#include <cmath>
#include <filesystem>
#include <QDebug>
#include <QKeyEvent>
#include <QDir>
#include <QStandardPaths>
#include <QFile>
#include <fstream>
#include <QTextStream>
#include <QMessageBox>
#include <QMimeData>
#include <QUrl>
#include <cmath>
#include <playlist.h>


/*Mix_Music* music = nullptr; // Initialize music to nullptr
Mix_Chunk* music3d=nullptr;
int isPlaying = false; // Track play/pause state
std::string currentSong = ""; // Track the currently playing song
QTimer *timer = nullptr; // Timer for updating progress bar
bool use3DSound = false;
*/
int channel=-1;

namespace fs = std::filesystem;

enum PlayMode { Loop, Sequential, Shuffle }; // Define play modes

PlayMode currentMode = Sequential; // Start with Sequential Mode

// Function to check if a file is a music file


// Function to find music files in a directory
void findMusicFiles(const fs::path& directory, std::vector<fs::path>& musicFiles) {
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cerr << "Skipping invalid or non-directory path: " << directory << std::endl;
        return;
    }
    try {
        for (const auto& entry : fs::recursive_directory_iterator(directory)) {
            // Skip directories and non-music files
            if (fs::is_regular_file(entry) && isMusicFile(entry.path())) {
                musicFiles.push_back(entry.path());
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error while accessing directory " << directory << ": " << e.what() << std::endl;
    }
}


// Function to get common directories for scanning music files
std::vector<fs::path> getDefaultMusicDirectories() {
    std::vector<fs::path> directories;

    // Add the user's Music directory (Windows specific)
    QString musicPath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    if (!musicPath.isEmpty()) {
        directories.push_back(musicPath.toStdString());
    }

    // Filter out some common system folders to avoid recursion issues
    QStringList excludedDirs = {"C:/Program Files", "C:/Windows", "C:/Users/Username/AppData"};
    directories.push_back(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).toStdString());
    directories.push_back(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString());
    directories.push_back(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation).toStdString());

    // Exclude specific directories if needed
    directories.erase(std::remove_if(directories.begin(), directories.end(),
                                     [&excludedDirs](const fs::path& p) {
                                         for (const auto& excluded : excludedDirs) {
                                             // Use find() to check if the directory starts with any excluded path
                                             std::string dirStr = p.string();
                                             if (dirStr.find(excluded.toStdString()) == 0) { // Checks if the path starts with excluded directory
                                                 return true;
                                             }
                                         }
                                         return false;
                                     }), directories.end());

    return directories;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    ui->modelabel->setPixmap(QPixmap(":/images/icons/sequence.png"));
    QSize labelSize = ui->modelabel->size();
    QPixmap pixmap = ui->modelabel->pixmap().scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->modelabel->setPixmap(pixmap);

    ui->searchicon->setPixmap(QPixmap(":/images/icons/search.png"));
    labelSize = ui->searchicon->size();
    pixmap = ui->searchicon->pixmap().scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->searchicon->setPixmap(pixmap);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateProgress);
    //connect(ui->Progressbar, &QSlider::sliderMoved, this, &MainWindow::seekMusic);
    connect(ui->Progressbar,  &QSlider::valueChanged, this, &MainWindow::seekMusic);
    ui->volumeslider->setRange(0, MIX_MAX_VOLUME);
    ui->volumeslider->setSliderPosition(MIX_MAX_VOLUME);
    ui->timelabel->setText("00:00"); // Set initial text for the timelabel
    ui->timelabel->setText("00:00");
    ui->SearchLine->setPlaceholderText("Search Songs...");

    connect(ui->SearchLine, &QLineEdit::textChanged, this, &MainWindow::on_searchTextChanged);
    connect(ui->sound3d, &QCheckBox::checkStateChanged, this, &MainWindow::toggle3DSound);
    // Automatically scan common directories when the app starts
    listfile();  // This will call listfile on startup
    listPlaylist();
    ui->play_btn->setIcon(QIcon(":/images/icons/play4.png"));
    ui->previous->setIcon(QIcon(":/images/icons/backward.png"));
    ui->next->setIcon(QIcon(":/images/icons/forward.png"));
    ui->end_btn->setIcon(QIcon(":/images/icons/close.png"));
    ui->browse_btn->setIcon(QIcon(":/images/icons/browse.png"));
    ui->browse_btn->setIconSize(ui->browse_btn->size() * 0.7);
    ui->delPlaylist->setIcon(QIcon(":/images/icons/delete.png"));
    ui->delSong->setIcon(QIcon(":/images/icons/delete.png"));
    ui->addtoPlaylist->setIcon(QIcon(":/images/icons/heart.png"));
    ui->addtoPlaylist->setIconSize(ui->addtoPlaylist->size() * 0.7);

    ui->music_label->setPixmap(QPixmap(":/images/icons/music1.png"));
    ui->music_label->setScaledContents(true);
    ui->sound_label->setPixmap(QPixmap(":/images/icons/volume.png"));
    ui->sound_label->setScaledContents(true);
    ui->sound3dlabel->setPixmap(QPixmap(":/images/icons/3d-audio.png"));
    ui->sound3dlabel->setScaledContents(true);
    ui->addplaylist_btn->setIcon(QIcon(":/images/icons/add.png"));
    ui->addplaylist_btn->setIconSize(ui->addplaylist_btn->size() * 0.9);

    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::updateButtonVisibility);

    updateButtonVisibility();
    connect(ui->playlistwidget, &QListWidget::itemSelectionChanged, this, &MainWindow::updateDeletelistVisibility);
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::updateDeletesongVisibility);
    updateDeletesongVisibility();
    updateDeletelistVisibility();


    ui->Refresh->setToolTip("All Songs");
    ui->play_btn->setToolTip("Play/Pause");
    ui->end_btn->setToolTip("Reset");
    ui->modebtn->setToolTip("Toggle playback mode");
    ui->sound3d->setToolTip("Enable 3D sound");
    ui->SearchLine->setToolTip("Search Songs");
    ui->addplaylist_btn->setToolTip("Add Playlist");
    ui->addtoPlaylist->setToolTip("Add song to playlist");
    ui->listWidget->setToolTip("Click to play the song");
    ui->playlistwidget->setToolTip("Click a playlist to open");
    ui->previous->setToolTip("Previous");
    ui->next->setToolTip("Next");
    ui->browse_btn->setToolTip("Browse folder");
    ui->delPlaylist->setToolTip("Delete Playlist");
    ui->delSong->setToolTip("Remove song from playlist");
    ui->modelabel->setToolTip("Sequential");


}

MainWindow::~MainWindow()
{
    if (music) {
        Mix_FreeMusic(music);
    }
    if(music3d){
        Mix_UnregisterAllEffects(channel);
        Mix_FreeChunk(music3d);      
    }
    delete ui;
}

void MainWindow::updateButtonVisibility() {
    if (ui->listWidget->selectedItems().isEmpty()) {
        ui->addtoPlaylist->setVisible(false); // Replace yourButton with the actual button name
    } else {
        ui->addtoPlaylist->setVisible(true); // Replace yourButton with the actual button name
    }
}

void MainWindow::updateDeletelistVisibility() {
    if (ui->playlistwidget->selectedItems().isEmpty()) {
        ui->delPlaylist->setVisible(false); // Replace yourButton with the actual button name
    } else {
        ui->delPlaylist->setVisible(true); // Replace yourButton with the actual button name
    }
}
void MainWindow::updateDeletesongVisibility() {
    if (ui->listWidget->selectedItems().isEmpty() || ui->playlistwidget->selectedItems().isEmpty()) {
        ui->delSong->setVisible(false); // Replace yourButton with the actual button name
    } else {
        ui->delSong->setVisible(true); // Replace yourButton with the actual button name
    }
}

void MainWindow::toggle3DSound(int state) {
    if (state == Qt::Checked) {
        use3DSound = true;
        ui->Progressbar->setEnabled(false);
        ui->music_label->setPixmap(QPixmap(":/images/icons/lock.png"));
    } else {
        use3DSound = false;
        ui->Progressbar->setEnabled(true);
        ui->music_label->setPixmap(QPixmap(":/images/icons/music1.png"));
    }
    ui->music_label->setScaledContents(true);
    on_end_btn_clicked();
}

void MainWindow::showPlaylistDialog() {
    bool ok;
    QString playlistName = QInputDialog::getText(this, tr("Create Playlist"), tr("Playlist Name:"), QLineEdit::Normal, QString(), &ok);
    if (ok && !playlistName.isEmpty()) {
        createplaylist(playlistName);
    }
}

void MainWindow::on_play_btn_clicked()
{   if(Mix_PlayingMusic()){
        if (Mix_PausedMusic()) {
            Mix_ResumeMusic();
            if(use3DSound){Mix_Resume(channel);}
            ui->play_btn->setIcon(QIcon(":/images/icons/pause2.png"));
        } else {
            Mix_PauseMusic();
            if(use3DSound){Mix_Pause(channel);}
            ui->play_btn->setIcon(QIcon(":/images/icons/play4.png"));
        }
    }
}


void MainWindow::on_end_btn_clicked()
{
    Mix_HaltMusic();
    Mix_HaltChannel(channel);
    if (music) {
        Mix_FreeMusic(music);
        music = nullptr;
        std::cout << "Music file freed." << std::endl;
    }if(music3d){
        Mix_FreeChunk(music3d);
        music3d = nullptr;
        std::cout << "Music3d file freed." << std::endl;
    }

    isPlaying = false; // Reset play state
    currentSong = ""; // Reset current song
    ui->playing_song->setText("-_-_-_-");
    timer->stop();
    ui->Progressbar->setValue(0);
    ui->timelabel->setText("00:00");
    ui->end_label->setText("00:00");
    on_Refresh_clicked();
}

static void AUD_Effects(int chan, void *stream, int len, void *udata)
{

    static float pan = 0.0f;
    static int panDirection = 1;
    static float speed = 0.005f;


    pan += panDirection * speed;

    if (pan >= 1.0f) {
        pan = 1.0f;
        panDirection = -1;
    }
    if (pan <= 0.0f) {
        pan = 0.0f;
        panDirection = 1;
    }


    float easedPan = (sin(pan * M_PI - M_PI / 2) + 1) / 2;


    int leftVolume = (1.0f - easedPan) * 255;
    int rightVolume = easedPan * 255;

    Mix_SetPanning(chan, leftVolume, rightVolume);
}


int MainWindow::play(const std::string& fullPath) {
    std::cout << "Attempting to load: " << fullPath << std::endl;

    if (music) {
        Mix_FreeMusic(music);
        music = nullptr;
    }

    music = Mix_LoadMUS(fullPath.c_str());
    if (!music) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        return 1;
    }
    std::cout << "Music loaded successfully." << std::endl;
    int loops = (currentMode == Loop) ? -1 : 1; // -1 for loop, 1 for sequential (no loop)
    if(use3DSound){
        music3d = Mix_LoadWAV(fullPath.c_str());
        if (!music3d) {
            std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
            return 1;
        }
        Mix_VolumeMusic(0);
        channel = Mix_PlayChannel(-1, music3d, loops);  // Automatically get the first available channel
        if (channel == -1) {
            std::cerr << "Failed to play sound effect: " << Mix_GetError() << std::endl;
            Mix_FreeChunk(music3d);
            return 1;
        }
        std::cout << "Playing music3d..." << std::endl;

        // Register the effect after the sound starts playing (for the correct channel)
        Mix_RegisterEffect(channel, AUD_Effects, NULL, NULL);
    }


    if (Mix_PlayMusic(music, loops) == -1) {
        std::cerr << "Failed to play music: " << Mix_GetError() << std::endl;
        Mix_FreeMusic(music);
        music = nullptr;
        return 1;
    }

    std::cout << "Playing music..." << std::endl;
    isPlaying = true; // Update play state
    ui->play_btn->setIcon(QIcon(":/images/icons/pause2.png"));
    // Update progress bar
    double duration = Mix_MusicDuration(music);
    ui->Progressbar->setRange(0, static_cast<int>(duration));
    timer->start(1000);

    return 0;
}


void MainWindow::updateProgress()
{
    if (music) {
        double currentPos = Mix_GetMusicPosition(music);
        ui->Progressbar->setValue(static_cast<int>(currentPos));

        // Format the current position and total duration
        int currentMinutes = static_cast<int>(currentPos) / 60;
        int currentSeconds = static_cast<int>(currentPos) % 60;
        double totalDuration = Mix_MusicDuration(music);
        int totalMinutes = static_cast<int>(totalDuration) / 60;
        int totalSeconds = static_cast<int>(totalDuration) % 60;

        // Update the timer label
        ui->timelabel->setText(QString("%1:%2")
                                   .arg(currentMinutes, 2, 10, QChar('0'))
                                   .arg(currentSeconds, 2, 10, QChar('0')));

        ui->end_label->setText(QString("%1:%2")
                                   .arg(totalMinutes, 2, 10, QChar('0'))
                                   .arg(totalSeconds, 2, 10, QChar('0')));
        // Handle the end of music for sequential mode
        if (currentMode == Sequential && currentPos >= totalDuration){
            int nextIndex = ui->listWidget->currentRow() + 1;

            // Check if we are at the last song in the list
            if (nextIndex >= ui->listWidget->count()) {
                // If we are at the last song, go back to the first song
                nextIndex = 0;
            }

            // Play the next song in the list
            ui->listWidget->setCurrentRow(nextIndex);
            on_listWidget_itemClicked(ui->listWidget->item(nextIndex)); // Play the next song
        }
        else if (currentMode == Shuffle && currentPos >= Mix_MusicDuration(music)) {
            int randomIndex = rand() % ui->listWidget->count();  // Random index
            ui->listWidget->setCurrentRow(randomIndex);
            on_listWidget_itemClicked(ui->listWidget->item(randomIndex));
        }
    }
}


void MainWindow::seekMusic(int position)
{
    if (Mix_SetMusicPosition(position) == -1) {
        std::cerr << "Failed to seek music: " << Mix_GetError() << std::endl;
    }
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    // Get the current row of the clicked item
    int current_row = ui->listWidget->currentRow();

    // Use a static variable to keep track of the previous row
    static int previous_row = -1;

    // Check if the clicked item is different from the previously clicked item
    if (previous_row != -1 && previous_row != current_row) {
        // Un-bold the previous item (if it's not the same as the current item)
        QListWidgetItem *previousItem = ui->listWidget->item(previous_row);
        if (previousItem) {
            QFont font = previousItem->font();
            font.setBold(false);  // Remove bold from the previous item
            previousItem->setFont(font);
        }
    }

    // Bold the current item
    QFont font = item->font();
    font.setBold(true);  // Set bold to true for the current clicked item
    item->setFont(font);

    // Update the label to reflect the current playing song
    QString songPath = item->data(Qt::UserRole).toString();
    ui->playing_song->setText(item->text());

    std::string songPathStr = songPath.toStdString();

    // If the same song is clicked again, toggle play/pause
    if (currentSong == songPathStr) {
        if (isPlaying) {
            Mix_PauseMusic();
            if(use3DSound){Mix_Pause(channel);}
        } else {
            Mix_ResumeMusic();
            if(use3DSound){Mix_Resume(channel);}
        }
        isPlaying = !isPlaying;  // Toggle the play state
    } else {
        // If a new song is clicked, stop the current song and play the new one
        if (isPlaying) {
            Mix_HaltMusic();
            if (music) {
                Mix_FreeMusic(music);
                music = nullptr;
            }
            if(use3DSound){
                Mix_HaltChannel(channel);
                if(music3d){
                    Mix_FreeChunk(music3d);
                    music3d = nullptr;
                }
            }
        }

        // Update the current song
        currentSong = songPathStr;

        if (play(songPathStr) != 0) {
            std::cerr << "Failed to play song: " << songPathStr << std::endl;
        }
    }

    // Set the current row as the previous row for next click
    previous_row = current_row;

    // Set the clicked item as the current item in the list
    ui->listWidget->setCurrentItem(item);
}


void MainWindow::on_searchTextChanged(const QString &text)
{
    // Loop through all items in the list
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        // Check if the item's text contains the search query (case-insensitive)
        if (item->text().contains(text, Qt::CaseInsensitive)) {
            item->setHidden(false); // Show item if it matches
        } else {
            item->setHidden(true);  // Hide item if it doesn't match
        }
    }
}


void MainWindow::listfile()
{
    std::vector<fs::path> musicFiles;
    std::vector<fs::path> musicDirectories = getDefaultMusicDirectories(); // Get the list of directories

    for (const auto& directory : musicDirectories) {
        findMusicFiles(directory, musicFiles); // Find music files in these directories
    }

    ui->listWidget->clear(); // Clear the current list
    for (const auto& file : musicFiles) {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(file.filename().string()));
        item->setData(Qt::UserRole, QString::fromStdString(file.string()));
        ui->listWidget->addItem(item);
    }
}

void MainWindow::on_Refresh_clicked()
{
    listfile();
    listPlaylist();
}

void MainWindow::on_volumeslider_valueChanged(int value)
{   if(!use3DSound){
        Mix_VolumeMusic(value);
    }
    else{
        Mix_Volume(channel,value);
    }
    if(value==0){
        ui->sound_label->setPixmap(QPixmap(":/images/icons/mute.png"));
    }
    else{
        ui->sound_label->setPixmap(QPixmap(":/images/icons/volume.png"));
    }
    ui->volume_label->setText(QString("%1").arg((value*100)/128));
}

void MainWindow::on_previous_clicked()
{
    int prev = ui->listWidget->currentRow() - 1;
    if(prev<0){
        prev = ui->listWidget->count() - 1;
    }
    ui->listWidget->setCurrentRow(prev);
    on_listWidget_itemClicked(ui->listWidget->item(prev));
}

void MainWindow::on_next_clicked()
{
    int next = ui->listWidget->currentRow() + 1;
    if (next >= ui->listWidget->count()) {
        // If we are at the last song, go back to the first song
        next = 0;
    }
    ui->listWidget->setCurrentRow(next);
    on_listWidget_itemClicked(ui->listWidget->item(next));
}


void MainWindow::on_addplaylist_btn_clicked()
{
    showPlaylistDialog();
}


void MainWindow::on_browse_btn_clicked()
{
    // Open a file dialog to select a folder
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select Music Folder"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // If the user selected a valid directory
    if (!directory.isEmpty()) {
        // Clear the current list
        //ui->listWidget->clear();

        // Find music files in the selected directory
        std::vector<fs::path> musicFiles;
        findMusicFiles(directory.toStdString(), musicFiles);

        // Add the music files to the list widget
        for (const auto& file : musicFiles) {
            QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(file.filename().string()));
            item->setData(Qt::UserRole, QString::fromStdString(file.string())); // Store full path as user data
            ui->listWidget->addItem(item);
        }

        // Optional: Notify the user
        QMessageBox::information(this, "Directory Selected", "Music files have been loaded.");
    }
}




// Override dragEnterEvent for the main window
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    // Check if the dragged data contains URLs (i.e., files)
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();  // Accept the drag if it's a file
    } else {
        event->ignore();  // Ignore the drag if it's not a file
    }
}

// Override dropEvent for the main window
void MainWindow::dropEvent(QDropEvent *event)
{
    // Get the list of dropped files
    QList<QUrl> droppedUrls = event->mimeData()->urls();

    // Iterate through each dropped file
    for (const QUrl &url : droppedUrls) {
        QString filePath = url.toLocalFile();  // Get the file path from the URL

        // Ensure the dropped file is a valid music file
        if (isMusicFile(fs::path(filePath.toStdString()))) {
            // Process the file (add to listWidget, etc.)
            QFileInfo fileInfo(filePath);
            QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName());
            item->setData(Qt::UserRole, filePath);  // Store the full file path as user data
            ui->listWidget->addItem(item);  // Add the file to the list widget
            on_listWidget_itemClicked(item);
        }
    }
}



void MainWindow::on_delSong_clicked()
{
    // Get the playlist name from the UI.
    QString playlist_name = ui->playlistwidget->currentItem()->text();

    // Construct the full file path for the playlist.
    QString playlistfile = QDir::homePath() + "/Music/Playlists/" + playlist_name + ".m3u";

    // Get the song path from the UI, assuming the song's path is stored in the UserRole data.
    QString songPath = ui->listWidget->currentItem()->data(Qt::UserRole).toString();

    // Open the playlist file for reading
    std::ifstream file(playlistfile.toStdString());
    if (!file.is_open()) {
        QMessageBox::critical(this, "Error", "Failed to open the playlist file for reading.");
        return;
    }

    // Read the entire content of the playlist file into a vector of strings
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);  // Read each line into the vector
    }
    file.close();

    // Find and remove the song path from the playlist
    auto it = std::find(lines.begin(), lines.end(), songPath.toStdString());
    if (it != lines.end()) {
        lines.erase(it);  // Remove the song path from the vector
    } else {
        QMessageBox::information(this, "Song Not Found", "The song is not in the playlist.");
        return;
    }

    // Open the playlist file for writing
    std::ofstream outFile(playlistfile.toStdString());
    if (!outFile.is_open()) {
        QMessageBox::critical(this, "Error", "Failed to open the playlist file for writing.");
        return;
    }

    // Write the updated content back to the playlist file
    for (const auto& updatedLine : lines) {
        outFile << updatedLine << std::endl;  // Write each line to the file
    }
    outFile.close();

    on_playlistwidget_itemClicked(ui->playlistwidget->currentItem());
    QMessageBox::information(this, "Success", "Song removed from playlist.");

    // Optionally, refresh the playlist display or update the UI
    // listPlaylist();
}




void MainWindow::on_delPlaylist_clicked()   {
    // Get the playlist name from the UI.
    QString playlist_name = ui->playlistwidget->currentItem()->text();

    // Construct the full file path.
    QString playlistfile = QDir::homePath() + "/Music/Playlists/" + playlist_name + ".m3u";

    // Convert to std::string for std::remove
    std::string filePath = playlistfile.toStdString();

    // Check if the file exists before attempting to delete
    QFile file(QString::fromStdString(filePath));
    if (file.exists()) {
        // Attempt to delete the file.
        if (std::remove(filePath.c_str()) == 0) {
            listPlaylist();
            QMessageBox::information(this, "Success", "Playlist deleted successfully.");
            if (ui->playlistwidget->count() > 0) {
                ui->playlistwidget->setCurrentRow(0);
                on_playlistwidget_itemClicked(ui->playlistwidget->item(0));
            }
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete the playlist.");
        }
    } else {
        QMessageBox::warning(this, "File Not Found", "The specified playlist does not exist.");
    }
}


void MainWindow::on_modebtn_clicked()
{
    if (currentMode == Sequential) {
        currentMode = Loop;
        ui->modelabel->setPixmap(QPixmap(":/images/icons/loop.png"));
        ui->modelabel->setToolTip("Loop");
    } else if(currentMode == Loop) {
        currentMode = Shuffle;
        ui->modelabel->setPixmap(QPixmap(":/images/icons/shuffle.png"));
        ui->modelabel->setToolTip("Shuffle");
    }
    else{
        currentMode = Sequential;
        ui->modelabel->setPixmap(QPixmap(":/images/icons/sequence.png"));
        ui->modelabel->setToolTip("Sequential");
    }
    QSize labelSize = ui->modelabel->size();
    QPixmap pixmap = ui->modelabel->pixmap().scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->modelabel->setPixmap(pixmap);
}

