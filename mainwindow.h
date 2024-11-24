#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QSlider>
#include <QLabel>
#include <QTimer>
#include <SDL_mixer.h>
#include <QInputDialog>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_play_btn_clicked();
    void on_end_btn_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_Refresh_clicked();
    void updateProgress();
    void seekMusic(int position);
    void listfile();
    void on_volumeslider_valueChanged(int value);

    void on_previous_clicked();

    void on_next_clicked();

    void on_addplaylist_btn_clicked();
    void showPlaylistDialog();
    void createplaylist(const QString&);

    void listPlaylist();
    QStringList getPlaylistNames();
    void on_playlistwidget_itemClicked(QListWidgetItem *item);

    void on_addtoPlaylist_clicked();

    void on_browse_btn_clicked();
    void updateButtonVisibility();
    void updateDeletelistVisibility();
    void updateDeletesongVisibility();

    void toggle3DSound(int);
    void on_delSong_clicked();

    void on_delPlaylist_clicked();

    void on_modebtn_clicked();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
private:
    Ui::MainWindow *ui;
    QTimer *timer;
    int play(const std::string&);
    int play3d(const std::string&);
    QLabel *timelabel;
    int metadata();

    void on_searchTextChanged(const QString &);
    void updateModeLabel();  // Update the mode label based on the current mode

};





#endif // MAINWINDOW_H
