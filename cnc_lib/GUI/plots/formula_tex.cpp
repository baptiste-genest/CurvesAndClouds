#include "formula_tex.h"
using namespace cnc;

const std::string formula::tex_url = "https://latex.codecogs.com/png.latex?\\dpi{600}";
const tex::tex_eol tex::endl;
formula::formula(const tex_stream &ts,formula_disposition d) : fd(d)
{
    latex = ts.latex + "\\end{matrix}";
    downloaded = false;

    download_tex_result();

    text_color = QColorConstants::Black;
}

void cnc::formula::plot(frame_draw_object& fdo)
{
    if (!downloaded)
        return;
    QRect F;
    float dezoom_ratio;
    if (fd == centered){
        dezoom_ratio = 0.95;
        F = QRect(0,0,fdo.fi.width,fdo.fi.height);
    }
    else {
        dezoom_ratio = 0.4;
        uint w = fdo.fi.width*(1-dezoom_ratio);
        uint h = fdo.fi.height*(1-dezoom_ratio);
        F = QRect(algo::get_bit(fd,0)*w,(1-algo::get_bit(fd,1))*h,fdo.fi.width*dezoom_ratio,fdo.fi.height*dezoom_ratio);
    }

    QPixmap tmp = formula_pix.scaled(fdo.fi.width*dezoom_ratio,fdo.fi.height*dezoom_ratio,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    fdo.painter.drawPixmap(F.center() - tmp.rect().center(),tmp);
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
    auto url_text =QString::fromStdString(tex_url + latex);
    const QUrl url = QUrl(url_text);
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

tex_matrix::tex_matrix(uint w, uint h, const std::vector<std::string> &val){
    chunk = "\\begin{pmatrix}";
    for (uint j = 0;j<h;j++){
        for (uint i = 0;i<w;i++){
            chunk += val[j*w+i];
            if (i != w-1)
            chunk += " & ";
        }
        chunk += "\\\\";
    }
    chunk += "\\end{pmatrix}";
}
