# invoke SourceDir generated makefile for mmw_mss.per4ft
mmw_mss.per4ft: .libraries,mmw_mss.per4ft
.libraries,mmw_mss.per4ft: package/cfg/mmw_mss_per4ft.xdl
	$(MAKE) -f D:\ti\workspace_v10\out_of_box_1843_mss/src/makefile.libs

clean::
	$(MAKE) -f D:\ti\workspace_v10\out_of_box_1843_mss/src/makefile.libs clean

