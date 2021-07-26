#include <curvesandcloud.h>

using namespace cnc;

namespace Game {
    #define N 50
    #define W N
    #define H N
    #define OUTSIDE_TEMP 0.f
    #define AVERAGING_FACTOR 0.1f;
    std::array<float,H*W> cells;
    std::array<float,H*W> buffer;
};

int ID(int i,int j){
    return i+j*W;
}

float& at(int i,int j){
    return Game::cells[ID(i,j)];
}

float ix(int i,int j){
    const static float half_width = W/2.f;
    if (j >= H){
        float x = (i-half_width)/half_width;
        return std::exp(-x*x*20)*5;
    }
    if (i < 0 ||i >= W || j < 0)
        return OUTSIDE_TEMP;
    return Game::cells[ID(i,j)];
}

void init() {
    for (int k = 0;k<W*H;k++)
        Game::cells[k] = 0.f;
}

void swap(int a,int b){
    float tmp = Game::cells[a];
    Game::cells[a] = Game::cells[b];
    Game::cells[b] = tmp;
}

typedef std::pair<int,float> cell;

cell get_cell(int i,int j){
    return {ID(i,j),ix(i,j)};
}

bool in_box(int i,int j){
    return i >= 0 && i < W && j >= 0 && j < H;
}

int advect_id(int i,int j){
    const float v = ix(i,j);
    if (in_box(i,j-1) && v > ix(i,j-1))
        return ID(i,j-1);
    if (in_box(i+1,j-1) && in_box(i-1,j-1)){
        std::array<cell,3> neigh = {get_cell(i,j),get_cell(i+1,j-1),get_cell(i-1,j-1)};
        std::sort(neigh.begin(),neigh.end(),[] (const cell& a,const cell& b){return a.second < b.second;});
        if (neigh[0].first != ID(i,j))
            return neigh[0].first;
    }
    if (in_box(i+1,j-1)&& v > ix(i+1,j-1))
        return ID(i+1,j-1);
    if (in_box(i-1,j-1)&& v > ix(i-1,j-1))
        return ID(i-1,j-1);
    if (in_box(i+1,j) && in_box(i-1,j)){
        std::array<cell,3> neigh = {get_cell(i,j),get_cell(i+1,j),get_cell(i-1,j)};
        std::sort(neigh.begin(),neigh.end(),[] (const cell& a,const cell& b){return a.second < b.second;});
        if (neigh[0].first != ID(i,j))
            return neigh[0].first;
    }
    if (in_box(i+1,j)&& v > ix(i+1,j))
        return ID(i+1,j);
    if (in_box(i-1,j)&& v > ix(i-1,j))
        return ID(i-1,j);
    return ID(i,j);
}

void advect() {
    for (int j = 0;j<H;j++){
        for (int i = 0;i<W;i++){
            int id = ID(i,j);
            swap(id,advect_id(i,j));
        }
    }
}

float get_local_avg(int i,int j){
    float avg = ix(i,j);
    avg += ix(i-1,j);
    avg += ix(i+1,j);
    avg += ix(i,j-1);
    avg += ix(i,j+1);
    const static float one_fifth = 1.f/5.f;
    return avg * one_fifth;
}

void avg() {
    Game::buffer = Game::cells;
    for (int j = 0;j<H;j++)
        for (int i = 0;i<W;i++)
            Game::buffer[ID(i,j)] += (get_local_avg(i,j)-ix(i,j))*AVERAGING_FACTOR;
    Game::cells = Game::buffer;
}

QColor plot(int i,int j) {
    float temp = algo::clamp(ix(i,j),0.f,1.f);
    return QColor::fromRgb(temp*255,0,(1-temp)*255);
}

void run(){
    //source();
    avg();
    advect();
}

int main(int argc, char *argv[])
{
    init();
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("thermal convection");
    PlotFrame* F= T->add_frame();
    F->set_nb_layer_per_second(20);
    int nb_iter = 1000;
    for (int k = 0;k<nb_iter;k++){
        run();
        F->add_layer()->new_figure_from_texturing(W,H,plot);
    }

    w.show();
    return App.exec();
}
