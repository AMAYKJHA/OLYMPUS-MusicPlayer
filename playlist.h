#ifndef PLAYLIST_H
#define PLAYLIST_H
#include "mainwindow.h"
#define SDL_MAIN_Handled
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <SDL_mixer.h>
#include <iostream>
#include <cmath>
#include <set>
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
#include <music.h>

#include <SDL.h>


Mix_Music* music = nullptr; // Initialize music to nullptr
Mix_Chunk* music3d=nullptr;
int isPlaying = false; // Track play/pause state
std::string currentSong = ""; // Track the currently playing song
QTimer *timer = nullptr; // Timer for updating progress bar
bool use3DSound = false;



namespace fs = std::filesystem;

bool isMusicFile(const fs::path& file) {
    std::vector<std::string> musicExtensions = {".mp3", ".wav", ".flac", ".ogg"};
    return std::find(musicExtensions.begin(), musicExtensions.end(), file.extension().string()) != musicExtensions.end();
}


void MainWindow::createplaylist(const QString& playlist_name) {
    QString playlistDir = QDir::homePath() + "/Music/Playlists/";
    fs::path dir(playlistDir.toStdString());

    // Check if the Playlists directory exists, and create it if it doesn't
    if (!fs::exists(dir)) {
        if (!fs::create_directories(dir)) {
            QMessageBox::warning(this, "Error", "Could not create Playlists directory");
            return;
        }
    }

    fs::path fileName = dir / (playlist_name.toStdString() + ".m3u");
    std::ofstream ofile(fileName);
    if (ofile) {
        // Adding an optional header for m3u files
        ofile << "#EXTM3U\n";
        ofile.close();
        std::cout << "file saved" << std::endl;
        listPlaylist();
        QMessageBox::information(this, "Success", "Playlist created successfully");
    } else {
        QMessageBox::warning(this, "Error", "Could not create playlist file");
    }
}


void MainWindow::listPlaylist() {
    std::vector<fs::path> playlistFiles;
    QString playlistDir = QDir::homePath() + "/Music/Playlists/";
    fs::path dir(playlistDir.toStdString());

    // Check if the Playlists directory exists
    if (!fs::exists(dir)) {
        return;
    }

    // Iterate through the playlist files and add them to the list
    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.path().extension() == ".m3u") {
            playlistFiles.push_back(entry.path());
        }
    }

    ui->playlistwidget->clear(); // Clear the current list

    // Add each playlist without the .m3u extension
    for (const auto& file : playlistFiles) {
        // Get the filename and remove the .m3u extension
        QString playlistName = QString::fromStdString(file.filename().string());
        playlistName.chop(4);  // Remove the .m3u extension

        // Create the list item and set its data
        QListWidgetItem* item = new QListWidgetItem(playlistName);
        item->setData(Qt::UserRole, QString::fromStdString(file.string()));  // Store full file path as user data
        ui->playlistwidget->addItem(item);
    }
}


void MainWindow::on_playlistwidget_itemClicked(QListWidgetItem *item) {
    // Retrieve the full file path of the selected playlist file
    QString playlistFilePath = item->data(Qt::UserRole).toString();
    qDebug() << "Attempting to open playlist file:" << playlistFilePath;

    // Open the playlist file
    std::ifstream file(playlistFilePath.toStdString());
    if (!file.is_open()) {
        QMessageBox::warning(this, "Error", "Could not open playlist file: " + playlistFilePath);
        qDebug() << "Failed to open file:" << playlistFilePath;
        return;
    }

    // Clear the current list of songs in the list widget
    ui->listWidget->clear();

    std::string line;
    std::vector<fs::path> musicFiles;

    // Read each line in the playlist file
    while (std::getline(file, line)) {
        QString qLine = QString::fromStdString(line);

        // Skip comments in the playlist (lines starting with '#')
        if (qLine.startsWith("#")) {
            continue;
        }

        // Convert the playlist line to fs::path
        fs::path songPath = fs::path(qLine.toStdString());

        // Check if the file is a valid music file
        if (isMusicFile(songPath)) {
            // Add valid music files to the list widget
            QFileInfo fileInfo(QString::fromStdString(songPath.string()));
            QListWidgetItem *songItem = new QListWidgetItem(fileInfo.fileName());
            songItem->setData(Qt::UserRole, QString::fromStdString(songPath.string())); // Store the full path as user data
            ui->listWidget->addItem(songItem);
        }
    }

    file.close();
}



QStringList MainWindow::getPlaylistNames() {
    QStringList playlistNames;
    QString playlistDir = QDir::homePath() + "/Music/Playlists/";
    fs::path dir(playlistDir.toStdString());

    if (fs::exists(dir)) {
        for (const auto& entry : fs::directory_iterator(dir)) {
            if (entry.path().extension() == ".m3u") {
                // Extract the playlist name without the ".m3u" extension
                QString playlistName = QString::fromStdString(entry.path().stem().string());
                playlistNames.append(playlistName);
            }
        }
    }
    return playlistNames;
}



void MainWindow::on_addtoPlaylist_clicked()
{
    // Get the selected songs from the listWidget (music library)
    QList<QListWidgetItem *> selectedItems = ui->listWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "No Song Selected", "Please select at least one song to add to a playlist.");
        return;
    }

    // Ask the user to select a playlist
    bool ok;
    QString playlistName = QInputDialog::getItem(this, "Select Playlist", "Select a Playlist to Add Songs to:", getPlaylistNames(), 0, false, &ok);
    if (!ok ) {
        //QMessageBox::warning(this, "No Playlist Selected", "You must select a playlist to add songs to.");
        return;
    }

    // Build the playlist file path
    QString playlistFilePath = QDir::homePath() + "/Music/Playlists/" + playlistName + ".m3u";

    // First, check if the playlist file exists and read the existing songs into a set
    std::ifstream playlistFile(playlistFilePath.toStdString());
    std::set<std::string> existingSongs;

    if (playlistFile.is_open()) {
        std::string line;
        while (std::getline(playlistFile, line)) {
            existingSongs.insert(line);  // Insert each song path into the set for easy lookup
        }
        playlistFile.close();  // Close after reading
    }

    // Now, open the playlist file in append mode to add new songs
    std::ofstream playlistFileOut(playlistFilePath.toStdString(), std::ios::app);
    if (!playlistFileOut.is_open()) {
        QMessageBox::warning(this, "Error", "Could not open playlist file for writing: " + playlistFilePath);
        return;
    }

    // Add the selected songs to the playlist, but skip duplicates
    for (auto item : selectedItems) {
        QString songPath = item->data(Qt::UserRole).toString();  // Get the full path from the user data

        // Check if the song is already in the playlist (to avoid duplicates)
        if (existingSongs.find(songPath.toStdString()) == existingSongs.end()) {
            // Correctly write the song path to the playlist file (convert QString to std::string)
            playlistFileOut << songPath.toStdString() << std::endl;
            existingSongs.insert(songPath.toStdString());  // Add to set to track this song
        } else {
            // Optionally notify that this song was skipped
            //QMessageBox::information(this, "Song Skipped", "The song " + songPath + " is already in the playlist.");
        }
    }

    playlistFileOut.close();  // Close the file after appending

    // Notify the user
    QMessageBox::information(this, "Songs Added", "Selected songs have been added to the playlist.");

    // Optionally, you can refresh the playlist view
    listPlaylist();
}



































#endif // PLAYLIST_H
