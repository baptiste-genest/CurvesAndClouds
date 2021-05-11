#include "formula_tex.h"
using namespace cnc;

const std::string formula::tex_url = "https://latex.codecogs.com/png.latex?\\dpi{600}";
const tex::tex_eol tex::endl;
formula::formula(const tex_stream &ts)
{
    latex = ts.latex + "\\end{matrix}";
    downloaded = false;

    download_tex_result();

    text_color = QColorConstants::Black;
}

void cnc::formula::plot(frame_draw_object& fdo)
{
    const static float dezoom_ratio = 0.95f;
    if (downloaded){
        QRect F(0,0,fdo.fi.width,fdo.fi.height);
        QPixmap tmp = formula_pix.scaled(fdo.fi.width*dezoom_ratio,fdo.fi.height*dezoom_ratio,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        fdo.painter.drawPixmap(F.center() - tmp.rect().center(),tmp);
    }
}

void formula::change_pixmap_color(QPixmap & P, const QColor &C)
{
    QImage img = P.toImage();
    for (uint j = 0;j<(uint)img.height();j++){
        for (uint i = 0;i<(uint)img.width();i++){
            if (img.pixelColor(i,j) == QColorConstants::Black)
                img.setPixelColor(i,j,C);
        }
    }
    P = P.fromImage(img);
}

void formula::download_tex_result()
{
        QNetworkAccessManager *man = new QNetworkAccessManager(this);
        const QUrl url = QUrl(QString::fromStdString(tex_url + latex));
        QNetworkRequest request(url);
        man->get(request);

        formula* ref = this;

        connect(man,&QNetworkAccessManager::finished ,[ref] (QNetworkReply* reply) {
            ref->formula_pix.loadFromData(reply->readAll());
            change_pixmap_color(ref->formula_pix,ref->text_color);
            ref->downloaded = true;
            ref->update();
        });
}

void formula::change_text_color(const QColor & C)
{
    text_color = C;
}

tex_system::tex_system(const std::vector<std::string> &S)
{
    chunk = "\\begin{cases}";
    for (const auto& s : S)
        chunk += s + " \\\\ ";
    chunk += "\\end{cases}";
}
