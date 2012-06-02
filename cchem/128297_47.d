format 75

componentcanvas 128297 component_ref 128297 // comp_chem
  draw_component_as_icon default show_component_req_prov default show_component_rea default show_stereotype_properties default
  xyzwh 605 416 2000 179 61
end
componentcanvas 128937 component_ref 128937 // report
  draw_component_as_icon default show_component_req_prov default show_component_rea default show_stereotype_properties default
  xyzwh 51 206 2000 179 61
end
componentcanvas 129577 component_ref 128809 // data_supplier
  draw_component_as_icon default show_component_req_prov default show_component_rea default show_stereotype_properties default
  xyzwh 52 79 2000 169 59
end
componentcanvas 129705 component_ref 132649 // data_source
  draw_component_as_icon default show_component_req_prov default show_component_rea default show_stereotype_properties default
  xyzwh 395 135 2000 169 59
end
arrowjunctioncanvas 129833 class_ref 432681 // basic_datum
  xyz 300 128 3005 label_xy 272 147
arrowjunctioncanvas 130217 class_ref 354729 // filter
  xyz 300 180 3005 label_xy 297 199
line 129961 ---O
  from ref 129705 z 1999 to ref 129833
line 130089 ---(
  from ref 129577 z 1999 to ref 129833
line 130345 ---O
  from ref 129705 z 1999 to ref 130217
line 130473 ---(
  from ref 129577 z 1999 to ref 130217
line 130601 ---(
  from ref 128937 z 1999 to ref 129833
end
