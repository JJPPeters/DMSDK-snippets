
// Create the plot
// a line plot is just a different view of a 2D image
unsigned int num_points = 100;
unsigned int num_lines = 2;
Gatan::DM::Image my_plot = Gatan::DM::RealImage("My line plot", 8, sz, num_lines);

// image must be shown before we do stuff with it
my_plot.GetOrCreateImageDocument().Show();

{
    // lock the data, I scope this so it can be released as soon as it is not needed
    Gatan::PlugIn::ImageDataLocker my_plot_locker = Gatan::PlugIn::ImageDataLocker(my_plot);
    // get pointer to data so we can modify it
    double* my_plot_data = static_cast<double*>(my_plot_locker.get());

    // set the data (data is row major order, each row is one plot line)
    for (int i = 0; i < sz; ++i) {
        my_plot_data[i] = i;
        my_plot_data[i + num_points] = num_points - i;
    }
}
// set x axis scale and units (0 corresponds to x)
my_plot.SetDimensionScale(0, 3.14);
my_plot.SetDimensionUnitString(0, "pis");

// set y axis scale and units (note this is "intensity" not a "dimension")
my_plot.SetIntensityScale(1);
my_plot.SetIntensityUnitString("i");

// Get the image's imagedisplay
Gatan::DM::ImageDisplay my_disp = my_plot.GetImageDisplay(0);

// now we make the image a line plot (3 == line plot display)               
my_disp.ChangeDisplayType(3);

// to rename the slices, need a ScriptObject (for whatever reason) and us the default name of Slice#
Gatan::DM::ScriptObject temp_script_obj;
// Slice0
Gatan::DM::ImageDisplayFindSliceIdByLabel(my_disp, "Slice0", temp_script_obj);
Gatan::DM::ImageDisplaySetSliceLabelById(my_disp, temp_script_obj, "Going up!");
//
Gatan::DM::ImageDisplayFindSliceIdByLabel(my_disp, "Slice1", temp_script_obj);
Gatan::DM::ImageDisplaySetSliceLabelById(my_disp, temp_script_obj, "Going down :(");

// for many of the settings, need a LinePlotmageDisplay
// get this by getting a token, recasting it and using it in a constructor
DM_LinePlotImageDisplayToken my_line_disp_ref = reinterpret_cast<DM_LinePlotImageDisplayToken>(my_disp.get());
Gatan::DM::LinePlotmageDisplay my_line_disp(my_line_disp_ref);

// can now show the legend
my_line_disp.SetLegendShown(true);

// can toggle background and grid, but there is no way to customise the colours as far as I can see
my_line_disp.SetBackgroundOn(true);
my_line_disp.SetGridOn(false);

unsigned int slice = 0;
// DrawingStyle
// 0 = nothing
// 1 = line
// 2 = fill
// 3 = line + fill
Gatan::DM::LinePlotImageDisplaySetSliceDrawingStyle(my_line_disp, slice, 2);

// component id = 0 for line, 1 for fill, rgb from 0.0-1.0
unsigned int component = 1;
Gatan::DM::LinePlotImageDisplaySetSliceComponentColor(my_line_disp, slice, component, 0.2, 0.2, 0.8);

// should be obvious
Gatan::DM::LinePlotImageDisplaySetSliceLineThickness(my_line_disp, slice, 2);

// bool sets if transparency is to be used, number (0.0-1.0) sets the transparency
Gatan::DM::LinePlotImageDisplaySetSliceTransparency(my_line_disp, slice, true, 0.2);

// position window as desired
long sx, sy;
Gatan::DM::Window win = my_plot.GetOrCreateImageDocument().GetWindow();
Gatan::DM::WindowGetFrameSize(win, &sx, &sy);

Gatan::DM::WindowSetFramePosition(win, 0, 0); // 0,0 is top left (function sets position of top left of window)
Gatan::DM::WindowSetFrameSize(win, sx, sy/2);
