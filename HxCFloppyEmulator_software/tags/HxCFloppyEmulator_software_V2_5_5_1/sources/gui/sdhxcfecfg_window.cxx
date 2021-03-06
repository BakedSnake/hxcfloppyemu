// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "sdhxcfecfg_window.h"
#include "cb_sdhxcfecfg_window.h"
#include "gui_strings.h"

void sdhxcfecfg_window::cb_OK_i(Fl_Button* o, void*) {
  ((Fl_Window*)(o->parent()))->hide();
}
void sdhxcfecfg_window::cb_OK(Fl_Button* o, void* v) {
  ((sdhxcfecfg_window*)(o->parent()->user_data()))->cb_OK_i(o,v);
}

sdhxcfecfg_window::sdhxcfecfg_window() {
  { window = new Fl_Double_Window(565, 290, getString(STR_SDSETTINGSWINDOW_0001));
    window->labelsize(12);
    window->user_data((void*)(this));
    { Fl_Group* o = new Fl_Group(1, 0, 315, 289, getString(STR_SDSETTINGSWINDOW_0002));
      o->box(FL_ENGRAVED_FRAME);
      o->labeltype(FL_EMBOSSED_LABEL);
      o->labelsize(10);
      o->align(Fl_Align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE));
      { Fl_Group* o = new Fl_Group(11, 30, 135, 120, getString(STR_SDSETTINGSWINDOW_0003));
        o->box(FL_SHADOW_BOX);
        o->labeltype(FL_EMBOSSED_LABEL);
        o->labelsize(10);
        o->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        { slider_uisound_level = new Fl_Slider(21, 50, 105, 15, getString(STR_SDSETTINGSWINDOW_0004));
          slider_uisound_level->tooltip(getString(STR_SDSETTINGSWINDOW_0005));
          slider_uisound_level->type(5);
          slider_uisound_level->labelsize(12);
          slider_uisound_level->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
          slider_uisound_level->align(Fl_Align(FL_ALIGN_TOP));
        } // Fl_Slider* slider_uisound_level
        { slider_stepsound_level = new Fl_Slider(21, 85, 105, 15, getString(STR_SDSETTINGSWINDOW_0006));
          slider_stepsound_level->tooltip(getString(STR_SDSETTINGSWINDOW_0007));
          slider_stepsound_level->type(5);
          slider_stepsound_level->labelsize(12);
          slider_stepsound_level->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
          slider_stepsound_level->align(Fl_Align(FL_ALIGN_TOP));
        } // Fl_Slider* slider_stepsound_level
        { slider_scrolltxt_speed = new Fl_Slider(21, 120, 105, 15, getString(STR_SDSETTINGSWINDOW_0008));
          slider_scrolltxt_speed->tooltip(getString(STR_SDSETTINGSWINDOW_0009));
          slider_scrolltxt_speed->type(5);
          slider_scrolltxt_speed->labelsize(12);
          slider_scrolltxt_speed->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
          slider_scrolltxt_speed->align(Fl_Align(FL_ALIGN_TOP));
        } // Fl_Slider* slider_scrolltxt_speed
        o->end();
      } // Fl_Group* o
      { Fl_Group* o = new Fl_Group(151, 30, 145, 75, getString(STR_SDSETTINGSWINDOW_0010));
        o->box(FL_SHADOW_BOX);
        o->labeltype(FL_EMBOSSED_LABEL);
        o->labelsize(10);
        o->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        { valslider_device_standby_timeout = new Fl_Value_Slider(163, 80, 120, 15, getString(STR_SDSETTINGSWINDOW_0011));
          valslider_device_standby_timeout->type(5);
          valslider_device_standby_timeout->labelsize(12);
          valslider_device_standby_timeout->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
          valslider_device_standby_timeout->align(Fl_Align(FL_ALIGN_TOP));
        } // Fl_Value_Slider* valslider_device_standby_timeout
        { valslider_device_backlight_timeout = new Fl_Value_Slider(163, 51, 120, 15, getString(STR_SDSETTINGSWINDOW_0012));
          valslider_device_backlight_timeout->type(5);
          valslider_device_backlight_timeout->labelsize(12);
          valslider_device_backlight_timeout->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
          valslider_device_backlight_timeout->align(Fl_Align(FL_ALIGN_TOP));
        } // Fl_Value_Slider* valslider_device_backlight_timeout
        o->end();
      } // Fl_Group* o
      { chk_loadlastloaded = new Fl_Check_Button(151, 112, 145, 18, getString(STR_SDSETTINGSWINDOW_0013));
        chk_loadlastloaded->down_box(FL_DOWN_BOX);
        chk_loadlastloaded->labelsize(11);
        chk_loadlastloaded->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
      } // Fl_Check_Button* chk_loadlastloaded
      { chk_disabediskdriveselector = new Fl_Check_Button(151, 131, 154, 17, getString(STR_SDSETTINGSWINDOW_0014));
        chk_disabediskdriveselector->down_box(FL_DOWN_BOX);
        chk_disabediskdriveselector->labelsize(11);
        chk_disabediskdriveselector->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
      } // Fl_Check_Button* chk_disabediskdriveselector
      { chk_force_loading_startupa = new Fl_Check_Button(3, 153, 310, 16, getString(STR_SDSETTINGSWINDOW_0015));
        chk_force_loading_startupa->down_box(FL_DOWN_BOX);
        chk_force_loading_startupa->labelsize(11);
        chk_force_loading_startupa->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
      } // Fl_Check_Button* chk_force_loading_startupa
      { chk_force_loading_startupb = new Fl_Check_Button(3, 170, 310, 16, getString(STR_SDSETTINGSWINDOW_0016));
        chk_force_loading_startupb->down_box(FL_DOWN_BOX);
        chk_force_loading_startupb->labelsize(11);
        chk_force_loading_startupb->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
      } // Fl_Check_Button* chk_force_loading_startupb
      { Fl_Group* o = new Fl_Group(1, 233, 315, 55, getString(STR_SDSETTINGSWINDOW_0017));
        o->box(FL_ENGRAVED_FRAME);
        o->labeltype(FL_EMBOSSED_LABEL);
        o->labelsize(10);
        o->align(Fl_Align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE));
        { chk_enable_autoboot_mode = new Fl_Check_Button(3, 249, 142, 18, getString(STR_SDSETTINGSWINDOW_0018));
          chk_enable_autoboot_mode->down_box(FL_DOWN_BOX);
          chk_enable_autoboot_mode->labelsize(11);
          chk_enable_autoboot_mode->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
        } // Fl_Check_Button* chk_enable_autoboot_mode
        { chk_force_loading_autoboot = new Fl_Check_Button(3, 268, 226, 18, getString(STR_SDSETTINGSWINDOW_0019));
          chk_force_loading_autoboot->down_box(FL_DOWN_BOX);
          chk_force_loading_autoboot->labelsize(11);
          chk_force_loading_autoboot->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
        } // Fl_Check_Button* chk_force_loading_autoboot
        { chk_preindex = new Fl_Check_Button(144, 249, 171, 18, getString(STR_SDSETTINGSWINDOW_0020));
          chk_preindex->down_box(FL_DOWN_BOX);
          chk_preindex->labelsize(11);
          chk_preindex->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
        } // Fl_Check_Button* chk_preindex
        o->end();
      } // Fl_Group* o
      { Fl_Group* o = new Fl_Group(1, 190, 315, 45, getString(STR_SDSETTINGSWINDOW_0021));
        o->box(FL_ENGRAVED_FRAME);
        o->labeltype(FL_EMBOSSED_LABEL);
        o->labelsize(10);
        o->align(Fl_Align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE));
        { chk_enable_indexed_mode = new Fl_Check_Button(3, 210, 132, 16, getString(STR_SDSETTINGSWINDOW_0022));
          chk_enable_indexed_mode->down_box(FL_DOWN_BOX);
          chk_enable_indexed_mode->labelsize(11);
          chk_enable_indexed_mode->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
        } // Fl_Check_Button* chk_enable_indexed_mode
        o->end();
      } // Fl_Group* o
      o->end();
    } // Fl_Group* o
    { Fl_Group* o = new Fl_Group(316, 0, 250, 234, getString(STR_SDSETTINGSWINDOW_0023));
      o->box(FL_ENGRAVED_FRAME);
      o->labeltype(FL_EMBOSSED_LABEL);
      o->labelsize(10);
      o->align(Fl_Align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE));
      { chk_hfr_autoifmode = new Fl_Check_Button(321, 55, 85, 16, getString(STR_SDSETTINGSWINDOW_0024));
        chk_hfr_autoifmode->down_box(FL_DOWN_BOX);
        chk_hfr_autoifmode->labelsize(12);
        chk_hfr_autoifmode->callback((Fl_Callback*)ifcfg_window_datachanged, (void*)(this));
        chk_hfr_autoifmode->when(FL_WHEN_CHANGED);
      } // Fl_Check_Button* chk_hfr_autoifmode
      { chk_hfe_doublestep = new Fl_Check_Button(321, 75, 85, 16, getString(STR_SDSETTINGSWINDOW_0025));
        chk_hfe_doublestep->down_box(FL_DOWN_BOX);
        chk_hfe_doublestep->labelsize(12);
        chk_hfe_doublestep->callback((Fl_Callback*)ifcfg_window_datachanged, (void*)(this));
        chk_hfe_doublestep->when(FL_WHEN_CHANGED);
      } // Fl_Check_Button* chk_hfe_doublestep
      { choice_hfeifmode = new Fl_Choice(357, 24, 160, 20, getString(STR_SDSETTINGSWINDOW_0026));
        choice_hfeifmode->down_box(FL_BORDER_BOX);
        choice_hfeifmode->labelsize(12);
        choice_hfeifmode->textsize(12);
        choice_hfeifmode->callback((Fl_Callback*)ifcfg_window_datachanged, (void*)(this));
        choice_hfeifmode->when(FL_WHEN_CHANGED);
      } // Fl_Choice* choice_hfeifmode
      { Fl_Group* o = new Fl_Group(316, 97, 250, 137, getString(STR_SDSETTINGSWINDOW_0027));
        o->box(FL_ENGRAVED_FRAME);
        o->labeltype(FL_EMBOSSED_LABEL);
        o->labelsize(10);
        o->align(Fl_Align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE));
        { choice_interfacemode_drva_cfg = new Fl_Choice(361, 130, 95, 20, getString(STR_SDSETTINGSWINDOW_0028));
          choice_interfacemode_drva_cfg->down_box(FL_BORDER_BOX);
          choice_interfacemode_drva_cfg->labelsize(12);
          choice_interfacemode_drva_cfg->textsize(10);
          choice_interfacemode_drva_cfg->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
          choice_interfacemode_drva_cfg->when(FL_WHEN_CHANGED);
        } // Fl_Choice* choice_interfacemode_drva_cfg
        { choice_pin02_drva = new Fl_Choice(361, 150, 95, 20, getString(STR_SDSETTINGSWINDOW_0029));
          choice_pin02_drva->down_box(FL_BORDER_BOX);
          choice_pin02_drva->labelsize(12);
          choice_pin02_drva->textsize(10);
          choice_pin02_drva->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
          choice_pin02_drva->when(FL_WHEN_CHANGED);
        } // Fl_Choice* choice_pin02_drva
        { choice_pin34_drva = new Fl_Choice(361, 170, 95, 20, getString(STR_SDSETTINGSWINDOW_0030));
          choice_pin34_drva->down_box(FL_BORDER_BOX);
          choice_pin34_drva->labelsize(12);
          choice_pin34_drva->textsize(10);
          choice_pin34_drva->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
          choice_pin34_drva->when(FL_WHEN_CHANGED);
        } // Fl_Choice* choice_pin34_drva
        { chk_enable_twodrives_emu = new Fl_Check_Button(321, 195, 125, 16,getString(STR_SDSETTINGSWINDOW_0031));
          chk_enable_twodrives_emu->down_box(FL_DOWN_BOX);
          chk_enable_twodrives_emu->labelsize(12);
          chk_enable_twodrives_emu->callback((Fl_Callback*)sdhxcfecfg_window_datachanged);
        } // Fl_Check_Button* chk_enable_twodrives_emu
        { chk_drvb_as_motoron = new Fl_Check_Button(321, 210, 240, 16,getString(STR_SDSETTINGSWINDOW_0032));
          chk_drvb_as_motoron->down_box(FL_DOWN_BOX);
          chk_drvb_as_motoron->labelsize(12);
          chk_drvb_as_motoron->callback((Fl_Callback*)sdhxcfecfg_window_datachanged);
        } // Fl_Check_Button* chk_drvb_as_motoron
        { choice_interfacemode_drvb_cfg = new Fl_Choice(466, 130, 95, 20);
          choice_interfacemode_drvb_cfg->down_box(FL_BORDER_BOX);
          choice_interfacemode_drvb_cfg->labelsize(12);
          choice_interfacemode_drvb_cfg->textsize(10);
          choice_interfacemode_drvb_cfg->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
          choice_interfacemode_drvb_cfg->when(FL_WHEN_CHANGED);
        } // Fl_Choice* choice_interfacemode_drvb_cfg
        { choice_pin02_drvb = new Fl_Choice(466, 150, 95, 20);
          choice_pin02_drvb->down_box(FL_BORDER_BOX);
          choice_pin02_drvb->labelsize(12);
          choice_pin02_drvb->textsize(10);
          choice_pin02_drvb->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
          choice_pin02_drvb->when(FL_WHEN_CHANGED);
        } // Fl_Choice* choice_pin02_drvb
        { choice_pin34_drvb = new Fl_Choice(466, 170, 95, 20);
          choice_pin34_drvb->down_box(FL_BORDER_BOX);
          choice_pin34_drvb->labelsize(12);
          choice_pin34_drvb->textsize(10);
          choice_pin34_drvb->callback((Fl_Callback*)sdhxcfecfg_window_datachanged, (void*)(this));
          choice_pin34_drvb->when(FL_WHEN_CHANGED);
        } // Fl_Choice* choice_pin34_drvb
        { Fl_Text_Display* o = new Fl_Text_Display(381, 130, 50, 5,getString(STR_SDSETTINGSWINDOW_0033));
          o->box(FL_NO_BOX);
          o->labelsize(12);
          o->textsize(12);
        } // Fl_Text_Display* o
        { Fl_Text_Display* o = new Fl_Text_Display(486, 130, 50, 5,getString(STR_SDSETTINGSWINDOW_0034));
          o->box(FL_NO_BOX);
          o->labelsize(12);
          o->textsize(12);
        } // Fl_Text_Display* o
        o->end();
      } // Fl_Group* o
      o->end();
    } // Fl_Group* o
    { Fl_Button* o = new Fl_Button(318, 235, 107, 25,getString(STR_SDSETTINGSWINDOW_0035));
      o->labelsize(12);
      o->callback((Fl_Callback*)sdhxcfecfg_window_bt_load, (void*)(this));
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(318, 262, 107, 25,getString(STR_SDSETTINGSWINDOW_0036));
      o->labelsize(12);
      o->callback((Fl_Callback*)sdhxcfecfg_window_bt_save, (void*)(this));
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(473, 262, 90, 25,getString(STR_COMMON_OK));
      o->labelsize(12);
      o->callback((Fl_Callback*)cb_OK, (void*)(this));
    } // Fl_Button* o
    window->end();
  } // Fl_Double_Window* window
}
