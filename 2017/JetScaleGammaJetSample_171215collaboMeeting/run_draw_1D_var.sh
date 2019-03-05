#!/bin/csh
#void draw_1D_var(string ihcalType = "SS310",int initfile=0,int endfile =1000, bool do_fit=false)

root -l -b -q draw_1D_var.C\(\"SS310\",\"50GeV\",0,1000,false\)
root -l -b -q draw_1D_var.C\(\"SS310\",\"20GeV\",0,1000,false\)
root -l -b -q draw_1D_var.C\(\"Al\",\"50GeV\",0,1000,false\)
root -l -b -q draw_1D_var.C\(\"Al\",\"20GeV\",0,1000,false\)
root -l -b -q draw_1D_var.C\(\"Al_uninst\",\"50GeV\",0,1000,false\)
root -l -b -q draw_1D_var.C\(\"Al_uninst\",\"20GeV\",0,1000,false\)
