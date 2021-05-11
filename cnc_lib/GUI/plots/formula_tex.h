/**
  * @file formula_tex.h defines classes to add latex text to your plots! (requires web connection)
  */
#ifndef FORMULA_TEX_H
#define FORMULA_TEX_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "plottable.h"
#include <string>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QPixmap>
#include <algorithm>

namespace cnc {

/**
 * @brief The tex_system class allow to display a serie of formulas in a latex system
 */
class tex_system{
public:
    std::string chunk;
    tex_system(const std::vector<std::string>& s);
};

/**
 * @brief The tex class indicated to the stream to interpret given text as tex string, in order to plot formulas
 */
class tex{
private:
    class tex_eol{
    public:
        tex_eol() {}
    };
    friend class tex_stream;
public:
    std::string text;
    tex(const std::string& s) : text(s) {}
    const static tex_eol endl;
};

/**
 * @brief The tex_stream class is the stream to type text/formulas for a formula plot WARNING, YOU MUST ESCAPE SPECIAL CHARACTERS
 */
class tex_stream {
private:
    std::string latex;

    inline void add_latex(const std::string& s) {
        latex += s;
    }
public:
    friend class formula;

    tex_stream() {
        latex = "\\begin{matrix} ";
    }

    tex_stream(const tex_stream& o){
        latex = o.latex;
    }

    void operator=(const tex_stream&) = delete;

    inline friend tex_stream& operator<<(tex_stream& ts,const tex_stream& o){
        ts.add_latex(o.latex);
        return ts;
    }

    inline friend tex_stream& operator<<(tex_stream& ts,const tex::tex_eol&){
        const static std::string EOL = " \\\\ ";
        ts.add_latex(EOL);
        return ts;
    }

    inline friend tex_stream& operator<<(tex_stream& ts,const std::string& s) {
        ts.add_latex("\\text{" + s + "}");
        return ts;
    }
    inline friend tex_stream& operator<<(tex_stream& ts,const tex& t) {
        ts.add_latex( " " + t.text + " ");
        return ts;
    }

    inline friend tex_stream& operator<<(tex_stream& ts,scalar x) {
        ts.add_latex(std::to_string(x));
        return ts;
    }

    inline friend tex_stream& operator<<(tex_stream& ts,int x) {
        ts.add_latex(std::to_string(x));
        return ts;
    }

    inline friend tex_stream& operator<<(tex_stream& ts,const tex_system& t) {
        ts.add_latex(t.chunk);
        return ts;
    }

};

/**
 * @brief The formula class is a plot to display latex style text/formulas, one can interact with it with the tex_stream which works as std::cout,
 * WARNING: since it works when an online API to generate latex, you must be connected to internet
 */
class formula : public Plottable
{
public:
    formula(const tex_stream& ts);
    void change_text_color(const QColor&);

    inline plot_type get_type() const override {
        return text;
    }
private:
    void download_tex_result();

    std::string latex;

    QPixmap formula_pix;
    bool downloaded;

    virtual void call_draw_axis(frame_draw_object&) const override {};
    virtual void compute_values(const frame_info&) override {};
    virtual void compute_value_range(const frame_info&) override  {};


    void plot(frame_draw_object&) override;

    bool tex_downloaded = false;

    const static std::string tex_url;

    QColor text_color;

    static void change_pixmap_color(QPixmap&,const QColor& C);
};


}


#endif // FORMULA_TEX_H
