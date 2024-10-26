#ifndef LAUNCHERWINDOW_H
#define LAUNCHERWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkRequest>

class SDLWidget;

QT_BEGIN_NAMESPACE
namespace Ui {
class LauncherWindow;
}
QT_END_NAMESPACE

class LauncherWindow : public QMainWindow
{
    Q_OBJECT

public:
    LauncherWindow(QWidget *parent = nullptr);
    ~LauncherWindow();

// signals:
//     void fullStringChanged();
protected:
    void showEvent(QShowEvent* event) override;

private:
    Ui::LauncherWindow *ui;
    QString quakeExePath;
    QString fullString;     // Quake path + all arguments
    QStringList args;
    QString modDir;
    size_t screenWidth;
    size_t screenHeight;
    bool fullscreen;
    bool vsync;
    bool aspect;
    size_t bpp;
    QString startMap;

    // Quaddicted website related
    QString url;


private slots:
    void on_pushButtonFindQExe_clicked();
    void on_pushButtonLaunchExe_clicked();
    void on_pushButtonFindMod_clicked();
    void on_pushButtonQSync_clicked();

    void on_radioButtonRogue_clicked();
    void on_radioButtonHipnotic_clicked();
    void on_radioButtonCustomGame_clicked();

    void on_comboBoxResolution_currentIndexChanged(int index);
    void on_comboBoxBPP_currentIndexChanged(int index);

    void on_checkBoxFullScreen_clicked();
    void on_checkBoxVSync_clicked();
    void on_checkBoxAspect_clicked();

    void update_textEditFullString();
    void on_textEditMap_textChanged();
};

#endif // LAUNCHERWINDOW_H
