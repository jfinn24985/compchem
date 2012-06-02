format 75

classcanvas 128047 class_ref 129967 // user
  class_drawing_mode default show_context_mode default show_stereotype_properties default
  xyz 141 98 3005
end
usecasecanvas 128175 usecase_ref 128559 // Select settings
  xyzwh 350 225 3005 64 32 label_xy 332 257
end
usecasecanvas 128303 usecase_ref 128047 // Configure machine
  xyzwh 128 317 3005 64 32 label_xy 96 349
end
usecasecanvas 128431 usecase_ref 128175 // Configure job
  xyzwh 352 376 3005 64 32 label_xy 339 408
end
usecasecanvas 128559 usecase_ref 128303 // Configure chain
  xyzwh 578 317 3005 64 32 label_xy 557 349
end
usecasecanvas 128687 usecase_ref 128431 // Create Script
  xyzwh 349 101 3005 64 32 label_xy 336 133
end
line 128815 --->
  from ref 128047 z 3004 label "request new script" xyz 201 94 3004 to ref 128687
simplerelationcanvas 128943 simplerelation_ref 128175
  from ref 128175 z 3004 stereotype "<<include>>" xyz 226 283 3000 to ref 128303
end
simplerelationcanvas 129071 simplerelation_ref 128303
  from ref 128175 z 3004 stereotype "<<include>>" xyz 337 315 3000 to ref 128431
end
simplerelationcanvas 129199 simplerelation_ref 128431
  from ref 128175 z 3004 stereotype "<<include>>" xyz 450 285 3000 to ref 128559
end
simplerelationcanvas 129327 simplerelation_ref 128047
  from ref 128687 z 3004 stereotype "<<include>>" xyz 336 177 3000 to ref 128175
end
end
