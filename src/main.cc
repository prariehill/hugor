#include <QDebug>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QFileDialog>
#include <QtPlugin>
#include <cstdlib>

extern "C" {
#include "heheader.h"
}
#include "happlication.h"
#include "settings.h"
#include "hugodefs.h"


#ifdef SOUND_SDL
// On some platforms, SDL redefines main in order to provide a
// platform-specific main() implementation.  However, Qt handles this too,
// so things can get weird.  We need to make sure main is not redefined so
// that Qt can find our own implementation and SDL will not try to do
// platform-specific initialization work (like launching the Cocoa event-loop
// or setting up the application menu on OS X, or redirecting stdout and stderr
// to text files on Windows), which would break things.
#ifdef main
#  undef main
#endif
#endif


#if defined(Q_WS_MAC) || defined(Q_WS_WIN)
Q_IMPORT_PLUGIN(qcncodecs)
Q_IMPORT_PLUGIN(qjpcodecs)
Q_IMPORT_PLUGIN(qtwcodecs)
Q_IMPORT_PLUGIN(qkrcodecs)
#endif


int main( int argc, char* argv[] )
{
    initSoundEngine();

    HApplication* app = new HApplication(argc, argv, "Hugor", "0.1",
                                         "Nikos Chantziaras", "");
    // Filename of the game to run.
    QString gameFileName;

    const QStringList& args = app->arguments();
    if (args.size() == 2) {
        if (QFile::exists(args.at(1))) {
            gameFileName = args.at(1);
        } else if (QFile::exists(args.at(1) + QString::fromAscii(".hex"))) {
            gameFileName = args.at(1) + QString::fromAscii(".hex");
        } else {
            qWarning() << "File" << args.at(1) << "not found.";
        }
    }

    if (gameFileName.isEmpty() and app->settings()->askForGameFile) {
        gameFileName = QFileDialog::getOpenFileName(0, QObject::tr("Choose the story file you wish to play"),
                                                    app->settings()->lastFileOpenDir,
                                                    QObject::tr("Hugo Games")
                                                    + QString::fromAscii("(*.hex *.Hex *.HEX)"));
    }
    int ret = 0;
    if (not gameFileName.isNull()) {
        QMetaObject::invokeMethod(app, "main", Qt::QueuedConnection, Q_ARG(QString, gameFileName));
        ret = app->exec();
    }
    delete app;
    closeSoundEngine();
    return ret;
}
