object_recognition.cpp
    Compile using:
    g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g object_recognition.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_stitching -lopencv_flann -lopencv_nonfree
    
    Run using:
    ./a.out <path_to_img1> ... <path_to_imgN>
    
    
object_recongition_single.cpp
    Compile using:
    g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g object_recognition_single.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_stitching -lopencv_flann -lopencv_nonfree
    
    Run using:
    ./a.out <path_to_img>

