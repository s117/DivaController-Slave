######################################################
#
# XPS Tcl API script generated by PlanAhead
#
######################################################

cd "D:/Bitman/EDA_Project/PlanAhead/DIVA_Project/DIVA_Project.srcs/sources_1/edk/system"
if { [xload new system.xmp] != 0 } {
  exit -1
}
xset arch virtex5
xset dev xc5vlx110t
xset package ff1136
xset speedgrade -1

xset binfo 

if { [xset hier sub] != 0 } {
  exit -1
}
xset hdl verilog
save proj
exit
