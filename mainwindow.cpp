
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




Mix_Music* music = nullptr;
Mix_Chunk* music3d=nullptr;
int isPlaying = false;
std::string currentSong = "";
QTimer *timer = nullptr;
bool use3DSound = false;

int channel=-1;

namespace fs = std::filesystem;

enum PlayMode { Loop, Sequential, Shuffle };

PlayMode currentMode = Sequential;




void findMusicFiles(const fs::path& directory, std::vector<fs::path>& musicFiles) {
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cerr << "Skipping invalid or non-directory path: " << directory << std::endl;
        return;
    }

    try {
        for (const auto& entry : fs::recursive_directory_iterator(directory)) {

            if (fs::is_regular_file(entry) && isMusicFile(entry.path())) {
                musicFiles.push_back(entry.path());
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error while accessing directory " << directory << ": " << e.what() << std::endl;
    }
}



std::vector<fs::path> getDefaultMusicDirectories() {
    std::vector<fs::path> directories;


    QString musicPath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    if (!musicPath.isEmpty()) {
        directories.push_back(musicPath.toStdString());
    }


    QStringList excludedDirs = {"C:/Program Files", "C:/Windows", "C:/Users/Username/AppData"};
    directories.push_back(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).toStdString());
    directories.push_back(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString());
    directories.push_back(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation).toStdString());


    directories.erase(std::remove_if(directories.begin(), directories.end(),
                                     [&excludedDirs](const fs::path& p) {
                                         for (const auto& excluded : excludedDirs) {

                                             std::string dirStr = p.string();
                                             if (dirStr.find(excluded.toStdString()) == 0) {
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
    connect(ui->Progressbar, &QSlider::sliderMoved, this, &MainWindow::seekMusic);
    ui->volumeslider->setRange(0, MIX_MAX_VOLUME);
    ui->volumeslider->setSliderPosition(MIX_MAX_VOLUME);
    ui->timelabel->setText("00:00");
    ui->timelabel->setText("00:00");
    ui->SearchLine->setPlaceholderText("Search Songs...");

    connect(ui->SearchLine, &QLineEdit::textChanged, this, &MainWindow::on_searchTextChanged);
    connect(ui->sound3d, &QCheckBox::checkStateChanged, this, &MainWindow::toggle3DSound);

    listfile();
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
        ui->addtoPlaylist->setVisible(false);
    } else {
        ui->addtoPlaylist->setVisible(true);
    }
}

void MainWindow::updateDeletelistVisibility() {
    if (ui->playlistwidget->selectedItems().isEmpty()) {
        ui->delPlaylist->setVisible(false);
    } else {
        ui->delPlaylist->setVisible(true);
    }
}
void MainWindow::updateDeletesongVisibility() {
    if (ui->listWidget->selectedItems().isEmpty() || ui->playlistwidget->selectedItems().isEmpty()) {
        ui->delSong->setVisible(false);
    } else {
        ui->delSong->setVisible(true);
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
    PlaylistManager plst;
    QString playlistName = QInputDialog::getText(this, tr("Create Playlist"), tr("Playlist Name:"), QLineEdit::Normal, QString(), &ok);
    if (ok && !playlistName.isEmpty()) {
        plst.createplaylist(playlistName);
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

    isPlaying = false;
    currentSong = "";
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
    int loops = (currentMode == Loop) ? -1 : 1;
    if(use3DSound){
        music3d = Mix_LoadWAV(fullPath.c_str());
        if (!music3d) {
            std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
            return 1;
        }
        Mix_VolumeMusic(0);
        channel = Mix_PlayChannel(-1, music3d, loops);
        if (channel == -1) {
            std::cerr << "Failed to play sound effect: " << Mix_GetError() << std::endl;
            Mix_FreeChunk(music3d);
            return 1;
        }
        std::cout << "Playing music3d..." << std::endl;


        Mix_RegisterEffect(channel, AUD_Effects, NULL, NULL);
    }


    if (Mix_PlayMusic(music, loops) == -1) {
        std::cerr << "Failed to play music: " << Mix_GetError() << std::endl;
        Mix_FreeMusic(music);
        music = nullptr;
        return 1;
    }

    std::cout << "Playing music..." << std::endl;
    isPlaying = true;
    ui->play_btn->setIcon(QIcon(":/images/icons/pause2.png"));

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


        int currentMinutes = static_cast<int>(currentPos) / 60;
        int currentSeconds = static_cast<int>(currentPos) % 60;
        double totalDuration = Mix_MusicDuration(music);
        int totalMinutes = static_cast<int>(totalDuration) / 60;
        int totalSeconds = static_cast<int>(totalDuration) % 60;


        ui->timelabel->setText(QString("%1:%2")
                                   .arg(currentMinutes, 2, 10, QChar('0'))
                                   .arg(currentSeconds, 2, 10, QChar('0')));

        ui->end_label->setText(QString("%1:%2")
                                   .arg(totalMinutes, 2, 10, QChar('0'))
                                   .arg(totalSeconds, 2, 10, QChar('0')));

        if (currentMode == Sequential && currentPos >= totalDuration){
            int nextIndex = ui->listWidget->currentRow() + 1;


            if (nextIndex >= ui->listWidget->count()) {

                nextIndex = 0;
            }


            ui->listWidget->setCurrentRow(nextIndex);
            on_listWidget_itemClicked(ui->listWidget->item(nextIndex));
        }
        else if (currentMode == Shuffle && currentPos >= Mix_MusicDuration(music)) {
            int randomIndex = rand() % ui->listWidget->count();
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

    int current_row = ui->listWidget->currentRow();


    static int previous_row = -1;


    if (previous_row != -1 && previous_row != current_row) {

        QListWidgetItem *previousItem = ui->listWidget->item(previous_row);
        if (previousItem) {
            QFont font = previousItem->font();
            font.setBold(false);
            previousItem->setFont(font);
        }
    }


    QFont font = item->font();
    font.setBold(true);
    item->setFont(font);


    QString songPath = item->data(Qt::UserRole).toString();
    ui->playing_song->setText(item->text());

    std::string songPathStr = songPath.toStdString();


    if (currentSong == songPathStr) {
        if (isPlaying) {
            Mix_PauseMusic();
            if(use3DSound){Mix_Pause(channel);}
        } else {
            Mix_ResumeMusic();
            if(use3DSound){Mix_Resume(channel);}
        }
        isPlaying = !isPlaying;
    } else {

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


        currentSong = songPathStr;

        if (play(songPathStr) != 0) {
            std::cerr << "Failed to play song: " << songPathStr << std::endl;
        }
    }


    previous_row = current_row;


    ui->listWidget->setCurrentItem(item);
}


void MainWindow::on_searchTextChanged(const QString &text)
{

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);

        if (item->text().contains(text, Qt::CaseInsensitive)) {
            item->setHidden(false);
        } else {
            item->setHidden(true);
        }
    }
}


void MainWindow::listfile()
{
    std::vector<fs::path> musicFiles;
    std::vector<fs::path> musicDirectories = getDefaultMusicDirectories();

    for (const auto& directory : musicDirectories) {
        findMusicFiles(directory, musicFiles);
    }

    ui->listWidget->clear();
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

    QString directory = QFileDialog::getExistingDirectory(this, tr("Select Music Folder"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);


    if (!directory.isEmpty()) {

        ui->listWidget->clear();


        std::vector<fs::path> musicFiles;
        findMusicFiles(directory.toStdString(), musicFiles);


        for (const auto& file : musicFiles) {
            QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(file.filename().string()));
            item->setData(Qt::UserRole, QString::fromStdString(file.string())); // Store full path as user data
            ui->listWidget->addItem(item);
        }


        QMessageBox::information(this, "Directory Selected", "Music files have been loaded.");
    }
}





void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{

    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}


void MainWindow::dropEvent(QDropEvent *event)
{

    QList<QUrl> droppedUrls = event->mimeData()->urls();


    for (const QUrl &url : droppedUrls) {
        QString filePath = url.toLocalFile();


        if (isMusicFile(fs::path(filePath.toStdString()))) {

            QFileInfo fileInfo(filePath);
            QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName());
            item->setData(Qt::UserRole, filePath);
            ui->listWidget->addItem(item);
            on_listWidget_itemClicked(item);
        }
    }
}



void MainWindow::on_delSong_clicked()
{

    QString playlist_name = ui->playlistwidget->currentItem()->text();


    QString playlistfile = QDir::homePath() + "/Music/Playlists/" + playlist_name + ".m3u";


    QString songPath = ui->listWidget->currentItem()->data(Qt::UserRole).toString();


    std::ifstream file(playlistfile.toStdString());
    if (!file.is_open()) {
        QMessageBox::critical(this, "Error", "Failed to open the playlist file for reading.");
        return;
    }


    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();


    auto it = std::find(lines.begin(), lines.end(), songPath.toStdString());
    if (it != lines.end()) {
        lines.erase(it);
    } else {
        QMessageBox::information(this, "Song Not Found", "The song is not in the playlist.");
        return;
    }


    std::ofstream outFile(playlistfile.toStdString());
    if (!outFile.is_open()) {
        QMessageBox::critical(this, "Error", "Failed to open the playlist file for writing.");
        return;
    }


    for (const auto& updatedLine : lines) {
        outFile << updatedLine << std::endl;
    }
    outFile.close();

    on_playlistwidget_itemClicked(ui->playlistwidget->currentItem());
    QMessageBox::information(this, "Success", "Song removed from playlist.");
    listPlaylist();
}




void MainWindow::on_delPlaylist_clicked()   {

    QString playlist_name = ui->playlistwidget->currentItem()->text();


    QString playlistfile = QDir::homePath() + "/Music/Playlists/" + playlist_name + ".m3u";


    std::string filePath = playlistfile.toStdString();


    QFile file(QString::fromStdString(filePath));
    if (file.exists()) {

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

