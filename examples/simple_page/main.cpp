#include <QCoreApplication>
#include <QFile>

#include "tcp/tcpmultithreadedserver.h"
#include "http/httpwebengine.h"
#include "http/bytearrayresource.h"

using namespace QtWebServer;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Tcp::MultithreadedServer s;
    Http::WebEngine w;

    w.addResource(new Http::ByteArrayResource(
                      "/",
                      QByteArray("Hello world")));

    s.setResponder(&w);
    s.listen(QHostAddress::Any, 3000);
    return a.exec();
}
