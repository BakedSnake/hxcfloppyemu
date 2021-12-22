// generated by Fast Light User Interface Designer (fluid) version 1.0300

#ifndef floppy_dump_window_h
#define floppy_dump_window_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Button.H>
extern void floppy_dump_ok(Fl_Button*, void*);
extern void floppy_dump_window_bt_read(Fl_Button*, void*);

class floppy_dump_window {
public:
  floppy_dump_window();
  Fl_Double_Window *window;
  Fl_Value_Input *number_of_track;
  Fl_Value_Input *number_of_retry;
  Fl_Round_Button *sel_drive_a;
  Fl_Round_Button *sel_drive_b;
  Fl_Check_Button *double_sided;
  Fl_Check_Button *double_step;
  Fl_Output *global_status;
  Fl_Output *current_status;
};
#endif
