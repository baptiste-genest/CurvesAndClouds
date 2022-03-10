**DOCUMENTATION NOT UP TO DATE**
# Curves and Clouds
Scientific computing library and data visualization

### Organisation
- apps : applications tournant avec Qt + C&C
- cnc_lib : code source de la lib
- data : assets pour certaines apps
- doc : documentation doxygen + Gantt + UML
- new_project : script bash pour création de projet Qt creator pour utiliser la lib
- README.md : ce fichier
- templates : fichers pour le script

### For the demo apps
In order to use the demo apps, you'll need to compile the library in a directory at the root folder named `debug`.
The script link to the debug folder, by default.


## Curves and clouds
**If you just want to analyze data from files without coding:**
you can simply use the GUI version of C&C by compiling and launching Cnc_GUI.

**If you just want the computing tools provided by the lib**, you can simply `#include <curvesandcloudCore.h>` **(you don't need Qt)**, you'll have access to integration schemes, linear algebra tools (vectors, matrix, system solvers and eigen-stuff), simple differential equation solvers, lagrangian interpolation and more! 


**If you want to use C&C to plot scientific data, here's how:**
If you want to be able to display and interact with data and plots, you must include <curvesandcloud.h> and use Qt.
If you use QtCreator, you can simply use the bash script as so (works for linux at least) : 
`./new_project.sh [your project name]`
to generate a new qt project (with the needed modules and includes) in the apps folder, then you can simply open the .pro in QtCreator and code!

**Qt version: 5.15** (in April 2021)

The Curves&Cloud library is dynamically linked, you have to compile the lib first and then indicate the directory when compiling your program.

### **How the lib works?**

First of all, we recommend you to check the apps/Cnc_demo program to see a detailled example of what you can do with C&C and how.

What's the difference between a Plot_window, a Plot_tab, a Plot_frame, a Plot_layer and a Plot?


![Alt Text](doc/TFL.gif?raw=true)

- A Plot_window is the main structure, it's the actual window poping on your screen, from it you can only add a Plot_tab with`add_tab(name of your tab)`, one Plot_tab is displayed at a time on a window.
- A Plot_tab is a tab class that you can name. From it you can create Plot_frames, to do so, you can either use `add_frame(width of the frame(default = 1),height of the frame(default 1))` it will add a frame at the first available spot where it fits, or you can specify the location by giving the coordinates of the upper-left corner of the frame as so:
`add_frame_at(x coord of the UL corner,y coord of the UL corner,width of the frame(default = 1),height of the frame(default 1))`
![Alt Text](doc/frame_system.png?raw=true)
- A Plot_frame corresponds to a box where the plots are actually displayed, a frame will display all its layers consecutively, you can add one layer to the frame by `add_layer()`, you can also specify the number of layers displayed each second by `set_nb_layer_per_second(frame rate)`
- A Plot_layer is the structure that contains the actual plots, when a layer is drawn it will draw all the plots at the same time on top of each other. From a layer you can finally add a plot! 

### **Main plot types: and how to add them (from a Plot_layer)**
- **function plot** plotting standard 1D scalar functions

`layer.new_function_plot(function to plot, range to plot between,plot axis policy (y range = x range or adaptative to min/max values of the function))`

`layer.new_function_plot(function to plot, range to plot between,y range (you specify y bounds))`

- **point cloud** plotting point cloud (if dimension of the points is above 2, they are projected in 2D by PCA)

`layer.new_point_cloud(cloud to plot, std::vector<uint> each point radius)`

`layer.new_point_cloud(cloud to plot, point radius if uniform)`
- **2D curve** plotting 2D curves either by specifing X coords then Y coords or by giving position of each points

`layer.new_2D_curve(each X coord, each Y coord)`

`layer.new_2D_curve(std::vector<vec> position of each point)`
- **Colormap** plotting 2D scalar fields by coloring each point proportionnaly to its value

`layer.new_colormap(function to plot, x range to plot between, y range to plot between,color policy (either from min(blue) to max(red), or from min (blue) to zero, from zero to max(red)))`
- **Level curves** plotting levels curves of a scalar field (either a certain number of slices or 1 specific level curve (can display algebraic curves this way!))

`layer.new_level_curve(function to plot, x range to plot between, y range to plot between,number of slices)`

`layer.new_level_curve_unique(function to plot, x range to plot between, y range to plot between,value of the level)`
- **Vector fields** plotting 2D vector fields

`layer.new_vector_field(vector function to plot, x range to plot between, y range to plot between,number of vector per side)`
- **figures** displaying images either by loading a file or by specifing a texture function

`layer.new_figure_from_file(path to file)`

`layer.new_figure_from_texturing(width,height,texture function)`
- **text frame** display latex given formulas (REQUIRES INTERNET)

`layer.add_text_frame_from_tex_stream(a tex_stream is something like cout except that it will only display text in a frame if placed here)`

`layer.add_text_frame(text if you just want a simple plain text, on a single line,without latex formulas)`

**How to use tex_stream** Since it's an important feature, here's how to use it:

```
tex_stream ts;//first you need to create a tex_stream object
ts << "simple text" << tex::endl;//to add simple text you can use it as std::cout`
ts << tex("\\int{0}{1}\\sin(t)dt");//to add equations you have to place your code in tex("...") YOU HAVE TO ESCAPE BACKSLASHES  ` 
ts << tex_system({"a + b = 0","2a - b = 1"}); //to display system of equations, put it in tex_system({"...","...",})
[your plot layer]->add_text_frame_from_tex_stream(ts);//finally add it to your layer!
```

### **Main C&C types**
- **vec** N-dimensionnal vector (from linear algebra not std lol), you can have the norm, the dot_product, etc... You can access to i-th data by x(i), you can initialize it by vec x(dim) or x(std::vector of values)

- **mat** NxM-dimensionnal matrix (can multiply mat-mat, mat-vec, can solve linear systems, find eigenvalues of symetric matrices) You can access to i,j data by M(i,j)

- **range** std::pair of float (used to indicate ranges to plot)

- **cloud** behind the hood it's a std::vector<vec> used to store points but it's required in all stat functions, you can apply a vector function to all its elements (cloud.apply(V) or could.apply_to_copy(V))

- **nodes** simple std::vector<float> but used as the standard type for calculus (you can create one by get_linspace for exemple)

C&c uses a lot functionnal programming so here are the **must important function types and examples**:

- **scalar_function_1D** standard functions such as f(x) = sin(x) , used in function_plot

- **scalar_function_2D** f(x,y) = sin(x*y) used in scalar_fields

- **scalar_function**   where x is a vec: f(x) = x.norm() or f(x) = x(0) + x(1)

- **vector_function_2D** f(x,y) = vec({y,-x}) used in vector_field plots

- **vector_function** f(x) = -x where x is a vec

### **The scientific computing modules:** 
The list of _what_ you can do, but _how_ you can do it is in the doxygen doc!

- **Statistics** C&C allows you to compute the following stuff from a cloud:
    - **the mean**
    - **PCA**
    - **PCA projector** (reduce the dimension of the cloud by projecting it along the main eigenvectors of the covariance matrix )
    - **Kernel-PCA projector** (replace covariance matrix by a kernel matrix, usefull to discriminate data when PCA doesn't work, you have to give a kernel function)
    - **k-means algorithm** allows you to compute clusters when you know how many they are.
- **Random variable simulation**, (produce a cloud)
    - **Random normal vector** each coordinates follows a gaussian
    - **Sample gaussian vector by Principal Componants** Does the reverse of PCA, you give the axis and associated variance
    - **Sample uniform vector** each coordinates follows a uniform law (in a N-dimensionnal rectangle)
    - **Sample on unit sphere** draws random vector from the unit sphere of any dimension
- **1D calculus**
    - **get_linspace** produces a nodes from a to b given dx or the number of nodes  
    - **build_1D_interpolator** returns a scalar_function_1D that interpolates between the given nodes with given values, allows you to plot functions from discrete samples
    - **integration schemes:** you have to generate a scheme object first to then integrate it over nodes
        - **left rectangle**
        - **right rectangle**
        - **mid_point**
        - **trapeziodal**
        - **simpson**
    - **Lagrange interpolation:**
        - you can build a **lagrange_interpolator** object from your samples and then use it to interpolate
        - you can also generate nodes with tchebytchev points on a given interval
- **vector calculus**
    - **gradient** : either from a scalar_function or a scalar_function_2D (so you can plot it)
    - **divergence** : from a 2D vector field builds the scalar field of divergence over the domain
    - **signed rot norm** : from a 2D vector field builds the scalar field of the signed norm of the curl over the domain
    - **Vector 2D Array**: class to store and compute on discrete sampled vector fields (for fluid simulation for instance)
        - **interpolation in 2D** : by bilinear interpolation
        - **helmoltz hodge decomposition** 
- **differential equations**  you have to generate the scheme first to then compute the solution
    - **ODEs**:
        - **euler explicit**
        - **runge kutta 2 (RK2)**
        - **runge kutta 4 (RK4)**
    - **PDEs**: (for the moment only in 1D with linear operator, in matrix form, such as the heat equation, advection,etc...)
        - **euler explicit**
        - **euler implicit**
        - **crank nicholson**
- **optimization** (WIP)
    - **1D newton method** to find x with f(x) = 0
    - **1D dichotomia algoritm**
    - **gradient descent** (for the moment only with fixed step)
