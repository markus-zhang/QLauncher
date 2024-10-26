#include "launcherwindow.h"
#include "./ui_launcherwindow.h"
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>

LauncherWindow::LauncherWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LauncherWindow)
{
    ui->setupUi(this);

    // Default executable path
    ui->textEditQExePath->setText(QString("C:/Quake/quakespasm.exe"));

    // This connects the default textChanged signal to the update_textEditFullString slot
    connect(ui->textEditQExePath, &QTextEdit::textChanged, this, &LauncherWindow::update_textEditFullString);

    // textEditModDir shows this->modDir, so it this changes we should call the slot
    connect(ui->textEditModDir, &QTextEdit::textChanged, this, &LauncherWindow::update_textEditFullString);

    // combobox Resolution initialization
    ui->comboBoxResolution->addItem(QString("800 x 600"));
    ui->comboBoxResolution->addItem(QString("1024 x 800"));
    ui->comboBoxResolution->addItem(QString("1440 x 900"));
    ui->comboBoxResolution->addItem(QString("1920 x 1080"));

    connect(ui->comboBoxResolution, &QComboBox::currentIndexChanged, this, &LauncherWindow::update_textEditFullString);


    this->screenHeight = 0;
    this->screenWidth = 0;

    // combobox BPP
    ui->comboBoxBPP->addItem(QString("32"));
    ui->comboBoxBPP->addItem(QString("16"));

    connect(ui->comboBoxBPP, &QComboBox::currentIndexChanged, this, &LauncherWindow::update_textEditFullString);

    this->bpp = 32;

    // full screen?
    this->fullscreen = ui->checkBoxFullScreen->isChecked();

    connect(ui->checkBoxFullScreen, &QCheckBox::clicked, this, &LauncherWindow::update_textEditFullString);

    // vsync?
    this->vsync = ui->checkBoxFullScreen->isChecked();

    // aspect?
    this->aspect = ui->checkBoxAspect->isChecked();

    this->update_textEditFullString();

    // QUADDICTED related
    this->url = QString("https://www.quaddicted.com/reviews/");
}

LauncherWindow::~LauncherWindow()
{
    delete ui;
}

void LauncherWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);

}

void LauncherWindow::on_pushButtonFindQExe_clicked()
{
    this->quakeExePath = QFileDialog::getOpenFileName(this, "Select Quake Executable File", "", "Executable Files (*.exe)");

    if (!this->quakeExePath.isEmpty())
    {
        ui->textEditQExePath->setText(quakeExePath);
        // emit fullStringChanged(this->quakeExePath);
    }
    else
    {
        ui->textEditQExePath->setText("");
        // emit fullStringChanged(this->quakeExePath);
    }
}

void LauncherWindow::on_pushButtonLaunchExe_clicked()
{
    if (this->quakeExePath.isEmpty())
    {
        QMessageBox::critical(this, "Error", "Please select Quake executable first.");
    }
    else
    {
        QProcess proc;
        proc.setProgram(this->quakeExePath);
        QFileInfo fi(this->quakeExePath);
        proc.setWorkingDirectory(fi.absolutePath());
        // Set arguments
        // Step 1: mod folder
        if (!this->modDir.isEmpty())
        {
            this->args << "-game" << this->modDir;
        }
        // Step 2: Add resolution
        this->args << "-height" << QString::number(this->screenHeight);
        this->args << "-width" << QString::number(this->screenWidth);

        // Step 3: Add bpp
        this->args << "-bpp" << QString::number(this->bpp);

        // Step 4: Add full screen (or not)
        if (this->fullscreen)
        {
            this->args << "-f";
        }

        // Step 5: Add start map if applicable
        if (!this->startMap.isEmpty())
        {
            this->args << "+map" << this->startMap;
        }

        proc.setArguments(this->args);
        proc.startDetached();
    }
}

void LauncherWindow::on_pushButtonQSync_clicked()
{
    // As a test, just download Quaddicted review page to same folder
}

void LauncherWindow::on_radioButtonRogue_clicked()
{
    // Disable the text box for manually entering the dir
    ui->textEditModDir->setDisabled(true);
    this->modDir = QString("rogue");
    ui->textEditModDir->setText(QString("rogue"));
}

void LauncherWindow::on_radioButtonHipnotic_clicked()
{
    // Disable the text box for manually entering the dir
    ui->textEditModDir->setDisabled(true);
    this->modDir = QString("hipnotic");
    ui->textEditModDir->setText(QString("hipnotic"));
}

void LauncherWindow::on_radioButtonCustomGame_clicked()
{
    // Enble the text box for manually entering the dir
    ui->textEditModDir->setDisabled(false);
    // Clear the previously set folder
    this->modDir.clear();
}

void LauncherWindow::on_pushButtonFindMod_clicked()
{
    // Open a dialog to pick a mod folder
    if (this->modDir.isEmpty())
    {
        QMessageBox::warning(this, "No Quake Executable Selected", "Please select the Quake executable first.");
    }
    else
    {
        // TODO: Add
    }
}

void LauncherWindow::on_comboBoxResolution_currentIndexChanged(int index)
{
    // TODO: Remove hardcoded values and find something else that is scalable
    switch (index)
    {
    case 0:
    {
        // 800 x 600
        this->screenWidth = 800;
        this->screenHeight = 600;
        break;
    }
    case 1:
    {
        // 1024 x 800
        this->screenWidth = 1024;
        this->screenHeight = 800;
        break;
    }
    case 2:
    {
        // 1440 x 900
        this->screenWidth = 1440;
        this->screenHeight = 900;
        break;
    }
    case 3:
    {
        // 1920 x 1080
        this->screenWidth = 1920;
        this->screenHeight = 1080;
        break;
    }
    default:
    {
        // 1024 x 800
        this->screenWidth = 1024;
        this->screenHeight = 800;
        break;
    }
    }
}

void LauncherWindow::on_comboBoxBPP_currentIndexChanged(int index)
{
    // TODO: Remove hardcoded values and find something else that is scalable
    switch (index)
    {
    case 0:
    {
        // 32bit
        this->bpp = 32;
        break;
    }
    case 1:
    {
        // 16bit
        this->bpp = 16;
        break;
    }
    default:
    {
        // 32bit
        this->bpp = 32;
        break;
    }
    }
}

void LauncherWindow::on_checkBoxFullScreen_clicked()
{
    this->fullscreen = !(this->fullscreen);
    this->update_textEditFullString();
}

void LauncherWindow::on_checkBoxVSync_clicked()
{
    this->vsync = !(this->vsync);
    this->update_textEditFullString();
}

void LauncherWindow::on_checkBoxAspect_clicked()
{
    this->aspect = !(this->aspect);
    this->update_textEditFullString();
}

void LauncherWindow::update_textEditFullString()
{
    // Use quakeExePath for a quick test
    // TODO: should use fullString in production
    // this->quakeExePath = ui->textEditQExePath->toPlainText();
    // ui->textEditFullString->setText(this->quakeExePath);

    // Real code
    // This slot should regenerate the full string with quakeExePath and modDir
    this->quakeExePath = ui->textEditQExePath->toPlainText();
    // Step 1: Add mod path
    this->fullString.clear();
    this->fullString = this->quakeExePath + QString(" -game ") + this->modDir;
    // Step 2: Add resolution
    this->fullString += QString(" -width ") + QString::number(this->screenWidth) + QString(" -height ") + QString::number(this->screenHeight);
    // Step 3: Add fullscreen, vsync and aspect
    if (this->fullscreen)
    {
        this->fullString += QString(" -f ");
    }
    // TODO: Vsync option is probably wrong
    if (!this->aspect)
    {
        this->fullString += QString(" vid_wait 0 ");
    }
    else
    {
        this->fullString += QString(" vid_wait 1 ");
    }
    // Step 4: Add BPP
    this->fullString += QString(" -bpp ") + QString::number(this->bpp) + QString(" ");

    // Step 5: Start map
    if (!this->startMap.isEmpty())
    {
        this ->fullString += QString(" +map ") + this->startMap + QString(" ");
    }

    ui->textEditFullString->setText(this->fullString);
}

void LauncherWindow::on_textEditMap_textChanged()
{
    // "+map <mapName>"
    this->startMap = ui->textEditMap->toPlainText();
    this->update_textEditFullString();
}
