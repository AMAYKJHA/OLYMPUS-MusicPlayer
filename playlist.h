#ifndef PLAYLIST_H
#define PLAYLIST_H


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



namespace fs = std::filesystem;

class PlaylistManager:public MainWindow{
public:
    void createplaylist(const QString&);
    QStringList getPlaylistNames();
    void addToPlaylist();

};


bool isMusicFile(const fs::path& file) {
    std::vector<std::string> musicExtensions = {".mp3", ".wav", ".flac", ".ogg"};
    return std::find(musicExtensions.begin(), musicExtensions.end(), file.extension().string()) != musicExtensions.end();
}



void PlaylistManager::createplaylist(const QString& playlist_name) {
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


    if (!fs::exists(dir)) {
        return;
    }


    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.path().extension() == ".m3u") {
            playlistFiles.push_back(entry.path());
        }
    }

    ui->playlistwidget->clear();


    for (const auto& file : playlistFiles) {

        QString playlistName = QString::fromStdString(file.filename().string());
        playlistName.chop(4);  // Removing .m3u extension


        QListWidgetItem* item = new QListWidgetItem(playlistName);
        item->setData(Qt::UserRole, QString::fromStdString(file.string()));  // Store full file path as user data
        ui->playlistwidget->addItem(item);
    }
}


void MainWindow::on_playlistwidget_itemClicked(QListWidgetItem *item) {

    QString playlistFilePath = item->data(Qt::UserRole).toString();
    qDebug() << "Attempting to open playlist file:" << playlistFilePath;


    std::ifstream file(playlistFilePath.toStdString());
    if (!file.is_open()) {
        QMessageBox::warning(this, "Error", "Could not open playlist file: " + playlistFilePath);
        qDebug() << "Failed to open file:" << playlistFilePath;
        return;
    }


    ui->listWidget->clear();

    std::string line;
    std::vector<fs::path> musicFiles;


    while (std::getline(file, line)) {
        QString qLine = QString::fromStdString(line);


        if (qLine.startsWith("#")) {
            continue;
        }


        fs::path songPath = fs::path(qLine.toStdString());


        if (isMusicFile(songPath)) {

            QFileInfo fileInfo(QString::fromStdString(songPath.string()));
            QListWidgetItem *songItem = new QListWidgetItem(fileInfo.fileName());
            songItem->setData(Qt::UserRole, QString::fromStdString(songPath.string())); // Store the full path as user data
            ui->listWidget->addItem(songItem);
        }
    }

    file.close();
}



QStringList PlaylistManager::getPlaylistNames() {
    QStringList playlistNames;
    QString playlistDir = QDir::homePath() + "/Music/Playlists/";
    fs::path dir(playlistDir.toStdString());

    if (fs::exists(dir)) {
        for (const auto& entry : fs::directory_iterator(dir)) {
            if (entry.path().extension() == ".m3u") {

                QString playlistName = QString::fromStdString(entry.path().stem().string());
                playlistNames.append(playlistName);
            }
        }
    }
    return playlistNames;
}



void MainWindow::on_addtoPlaylist_clicked()
{
    PlaylistManager plst;
    QList<QListWidgetItem *> selectedItems = ui->listWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "No Song Selected", "Please select at least one song to add to a playlist.");
        return;
    }

    // Ask the user to select a playlist
    bool ok;
    QString playlistName = QInputDialog::getItem(this, "Select Playlist", "Select a Playlist to Add Songs to:", plst.getPlaylistNames(), 0, false, &ok);
    if (!ok ) {
        //QMessageBox::warning(this, "No Playlist Selected", "You must select a playlist to add songs to.");
        return;
    }


    QString playlistFilePath = QDir::homePath() + "/Music/Playlists/" + playlistName + ".m3u";


    std::ifstream playlistFile(playlistFilePath.toStdString());
    std::set<std::string> existingSongs;

    if (playlistFile.is_open()) {
        std::string line;
        while (std::getline(playlistFile, line)) {
            existingSongs.insert(line);
        }
        playlistFile.close();
    }


    std::ofstream playlistFileOut(playlistFilePath.toStdString(), std::ios::app);
    if (!playlistFileOut.is_open()) {
        QMessageBox::warning(this, "Error", "Could not open playlist file for writing: " + playlistFilePath);
        return;
    }


    for (auto item : selectedItems) {
        QString songPath = item->data(Qt::UserRole).toString();


        if (existingSongs.find(songPath.toStdString()) == existingSongs.end()) {

            playlistFileOut << songPath.toStdString() << std::endl;
            existingSongs.insert(songPath.toStdString());
        } else {

            QMessageBox::information(this, "Song Skipped", "The song is already in the playlist.");
        }
    }

    playlistFileOut.close();


    QMessageBox::information(this, "Songs Added", "Selected songs have been added to the playlist.");


    listPlaylist();
}



#endif // PLAYLIST_H
