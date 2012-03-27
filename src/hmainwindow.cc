#include <QDebug>
#include <QMenuBar>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLayout>
#include <QTextEdit>
#include <QTextCodec>
#include <QScrollBar>
#include <QLabel>

#include "hmainwindow.h"
#include "happlication.h"
#include "hframe.h"
#include "hscrollback.h"
#include "hmarginwidget.h"
#include "confdialog.h"
#include "aboutdialog.h"
#include "settings.h"
#include "hugodefs.h"
extern "C" {
#include "heheader.h"
}

class HMainWindow* hMainWin = 0;


HMainWindow::HMainWindow( QWidget* parent )
    : QMainWindow(parent),
      fConfDialog(0),
      fAboutDialog(0)
{
    Q_ASSERT(hMainWin == 0);

    // We make our menu bar parentless so it will be shared by all our windows
    // in Mac OS X.
    QMenuBar* menuBar = new QMenuBar(0);

    QMenu* menu;
    QAction* act;

    // "Edit" menu.
    menu = menuBar->addMenu(tr("&Edit"));
    act = new QAction(tr("&Preferences..."), this);
#if QT_VERSION >= 0x040600
    act->setIcon(QIcon::fromTheme(QString::fromAscii("configure")));
    act->setShortcuts(QKeySequence::Preferences);
#endif
    menu->addAction(act);
    connect(act, SIGNAL(triggered()), SLOT(fShowConfDialog()));

    act = new QAction(tr("Show &Scrollback"), this);
    act->setMenuRole(QAction::ApplicationSpecificRole);
    menu->addAction(act);
    connect(act, SIGNAL(triggered()), SLOT(showScrollback()));

    // "Help" menu.
    menu = menuBar->addMenu(tr("&Help"));
    act = new QAction(tr("A&bout Hugor"), this);
#if QT_VERSION >= 0x040600
    act->setIcon(QIcon::fromTheme(QString::fromAscii("help-about")));
#endif
    menu->addAction(act);
    connect(act, SIGNAL(triggered()), SLOT(fShowAbout()));

    this->setMenuBar(menuBar);
    this->fScrollbackWindow = new HScrollbackWindow(0);

    // Use a sane minimum size; by default Qt would allow us to be resized
    // to almost zero.
    this->setMinimumSize(240, 180);

    hMainWin = this;
}


void
HMainWindow::fShowConfDialog()
{
    // If the dialog is already open, simply activate and raise it.
    if (this->fConfDialog != 0) {
        this->fConfDialog->activateWindow();
        this->fConfDialog->raise();
        return;
    }
    this->fConfDialog = new ConfDialog(this);
    this->fConfDialog->setWindowTitle(tr("Hugor Preferences"));
    connect(this->fConfDialog, SIGNAL(finished(int)), this, SLOT(fHideConfDialog()));
#ifdef Q_WS_MAC
    // There's a bug in Qt for OS X that results in a visual glitch with
    // QFontComboBox widgets inside QFormLayouts.  Making the dialog 4 pixels
    // higher fixes it.
    //
    // See: http://bugreports.qt.nokia.com/browse/QTBUG-10460
    this->fConfDialog->layout()->activate();
    this->fConfDialog->setMinimumHeight(this->fConfDialog->minimumHeight() + 4);
#endif
    this->fConfDialog->show();
}


void
HMainWindow::fHideConfDialog()
{
    if (this->fConfDialog != 0) {
        this->fConfDialog->deleteLater();
        this->fConfDialog = 0;
    }
}


void
HMainWindow::fShowAbout()
{
    // If the dialog is already open, simply activate and raise it.
    if (this->fAboutDialog != 0) {
        this->fAboutDialog->activateWindow();
        this->fAboutDialog->raise();
        return;
    }

    this->fAboutDialog = new AboutDialog(this);
    connect(this->fAboutDialog, SIGNAL(finished(int)), SLOT(fHideAbout()));
    this->fAboutDialog->show();
}


void
HMainWindow::fHideAbout()
{
    if (this->fAboutDialog != 0) {
        this->fAboutDialog->deleteLater();
        this->fAboutDialog = 0;
    }
}


void
HMainWindow::showScrollback()
{
    // If we need to overlay the scrollback, remove the game window from
    // view and replace it with the scrollback window if we haven't already
    // done so.
    if (hApp->settings()->overlayScrollback) {
        if (hApp->marginWidget()->layout()->indexOf(this->fScrollbackWindow) < 0) {
            this->fScrollbackWindow->setWindowFlags(Qt::Widget);
            hApp->marginWidget()->removeWidget(hFrame);
            hApp->marginWidget()->setContentsMargins(0, 0, 0, 0);
            hApp->marginWidget()->addWidget(this->fScrollbackWindow);
            hApp->marginWidget()->setPalette(QApplication::palette(hApp->marginWidget()));

            // Add a banner at the top so the user knows how to exit
            // scrollback mode.
            QLabel* banner = new QLabel(this->fScrollbackWindow);
            banner->setAlignment(Qt::AlignCenter);
            banner->setContentsMargins(0, 3, 0, 3);

            // Display the banner in reverse colors so that it's visible
            // regardless of the desktop's color theme.
            QPalette pal = banner->palette();
            pal.setColor(QPalette::Window, this->fScrollbackWindow->palette().color(QPalette::WindowText));
            pal.setBrush(QPalette::WindowText, this->fScrollbackWindow->palette().color(QPalette::Base));
            banner->setPalette(pal);
            banner->setAutoFillBackground(true);

            // Make the informational message clickable by displaying it as a
            // link that closes the scrollback when clicked.
            QString bannerText("<html><style>a {text-decoration: none; color: ");
            bannerText += pal.color(QPalette::WindowText).name()
                       +  ";}</style><body>Scrollback (<a href='close'>Esc, "
                       +  QKeySequence(QKeySequence::Close).toString(QKeySequence::NativeText)
                       +  " or click here to exit</a>)</body></html>";
            banner->setText(bannerText);
            connect(banner, SIGNAL(linkActivated(QString)), SLOT(hideScrollback()));

            hApp->marginWidget()->setBannerWidget(banner);
            hFrame->hide();
            this->fScrollbackWindow->show();
            this->fScrollbackWindow->setFocus();
        }
        return;
    }

    // No overlay was requested.  If the scrollback is currently in its
    // overlay mode, make it a regular window again.
    if (this->fScrollbackWindow->windowType() == Qt::Widget) {
        this->hideScrollback();
        this->fScrollbackWindow->setParent(0);
        this->fScrollbackWindow->setWindowFlags(Qt::Window);
    }
    this->fScrollbackWindow->show();
    this->fScrollbackWindow->activateWindow();
    this->fScrollbackWindow->raise();
}


void
HMainWindow::hideScrollback()
{
    // We only have cleanup work if a scrollback overlay is currently active.
    if (hApp->marginWidget()->layout()->indexOf(this->fScrollbackWindow) < 0) {
        return;
    }

    hApp->marginWidget()->setBannerWidget(0);
    hApp->marginWidget()->removeWidget(this->fScrollbackWindow);
    this->fScrollbackWindow->hide();
    hApp->updateMargins(::bgcolor);
    hApp->marginWidget()->addWidget(hFrame);
    hFrame->show();
    hFrame->setFocus();
}


void
HMainWindow::closeEvent( QCloseEvent* e )
{
    if (not hApp->gameRunning()) {
        QMainWindow::closeEvent(e);
        return;
    }

    QMessageBox* msgBox = new QMessageBox(QMessageBox::Question,
                                          tr("Quit Hugor"),
                                          tr("Abandon the story and quit the application?"),
                                          QMessageBox::Yes | QMessageBox::Cancel, this);
    msgBox->setDefaultButton(QMessageBox::Cancel);
    msgBox->setInformativeText(tr("Any unsaved progress in the story will be lost."));
#ifdef Q_WS_MAC
    msgBox->setIconPixmap(QPixmap(":/he_32-bit_72x72.png"));
    // This presents the dialog as a sheet in OS X.
    msgBox->setWindowModality(Qt::WindowModal);
#endif

    if (msgBox->exec() == QMessageBox::Yes) {
        hApp->settings()->saveToDisk();
        closeSoundEngine();
        exit(0);
    } else {
        e->ignore();
    }
}


void
HMainWindow::appendToScrollback( const QByteArray& str )
{
    if (str.isEmpty()) {
        return;
    }

    this->fScrollbackWindow->moveCursor(QTextCursor::End);
    this->fScrollbackWindow->insertPlainText(hApp->hugoCodec()->toUnicode(str));
    this->fScrollbackWindow->verticalScrollBar()->triggerAction(QScrollBar::SliderToMaximum);
}
