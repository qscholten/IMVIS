#include "Parameters.h"
#include "opencv2/highgui.hpp"
#include <opencv2/videoio.hpp>  // Nodig om camera te kunnen gebruiken
#include "Histogram.h"
#include "Mirror.h"
#include <iostream>
#include "Invert.h"
#include "Contrast.h"
#include "Zoom.h"
#include "Rotate.h"
#include "VerdelingEnContrast.h"
#include "Filter.h"
#include "Mier.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace cv;
using namespace std;

// Hieronder (na de demo) per opdracht een functie, zie de main() onderaan voor de indeling.

int demo_met_camera() { // -1. Puur voor de demo, met histogram. Deze functie is geen onderdeel van de opdrachten.
    String destination_window = "Camerabeeld";
    Mat frame;
    cout << "Open camera... Onderstaande Debug INFO kun je negeren.\n" << endl;
    VideoCapture capture(0); // open de eerste camera die ik tegenkom (of vul zelf een nr in (>0))
    if (!capture.isOpened())
    {
        cerr << "FOUT: Kan camera capture niet starten. Is camera aangesloten?" << endl;
        return 1;
    }

    capture >> frame; // lees het volgende beeld van de camera
    Histogram histogram("Histogram src", frame);

    cout << "\nCamerabeeld info; breedte: " << capture.get(CAP_PROP_FRAME_WIDTH);
    cout << ", hoogte: " << capture.get(CAP_PROP_FRAME_HEIGHT);
    cout << ", Beelden per seconde: " << capture.get(CAP_PROP_FPS) << endl;
    cout << "Je kunt zelf een plaatje opslaan (zorg zelf in de code dat het pad klopt):" << endl;
    cout << "\tDruk 'g' voor *.pgm (grijswaarden)" << endl;
    cout << "\tDruk 'p' voor *.ppm (kleur)" << endl;
    cout << "\tDruk 'j' voor *.jpg (kleur)" << endl;
    cout << endl << "Om te stoppen: zorg dat een grafisch window geselecteerd is en druk 'ESC' om te stoppen." << endl;
    
    namedWindow(destination_window, WINDOW_AUTOSIZE);
    imshow(destination_window, frame);
    moveWindow(destination_window, 0, 0); // Plaatje naar linksboven verplaatsen
    moveWindow("Histogram src", 89, 513); // Histogram onder plaatje schuiven

    while (1) {
        capture >> frame; // lees het volgende beeld van de camera
        if (frame.empty())
        {
            cerr << "FOUT: Kan geen beelden van camera ontvangen." << endl;
            break;
        }
        else {
            imshow(destination_window, frame); // Dit is het enige dat je nodig hebt. De namedWindow() en moveWindow() hierboven mag je weglaten.
        }
        int key = waitKey(1); // wacht 1 mSec tot toets ingedrukt is. 0 = altijd wachten.
        if (key == 'g') { // *.pgm plaatje opslaan; grijswaarden
            Mat gray_image; 
            cvtColor(frame, gray_image, COLOR_RGB2GRAY); // Converteer (kleuren) bronplaatje naar 8 bits per pixel grijswaarden plaatje.
            vector<int> compression_params;
            compression_params.push_back(IMWRITE_PXM_BINARY); // https://en.wikipedia.org/wiki/Netpbm
            compression_params.push_back(1); // 0 => grijswaarden dus *.pgm, 1 => kleurenplaatje dus *.ppm
            imwrite("C://Users//Kwik//Documents//HBO-ICT//Semester 4//Kwartaal 4//Computer Vision//lab_materialen//lab1//donker.pgm", gray_image); // de bestandsextensie zegt nu niks over de inhoud dus zorg dat deze klopt
        }
        if (key == 'p') {// *.ppm plaatje opslaan; kleur
            vector<int> compression_params;
            compression_params.push_back(IMWRITE_PXM_BINARY);
            compression_params.push_back(1);
            imwrite("C://Users//Kwik//Documents//HBO-ICT//Semester 4//Kwartaal 4//Computer Vision//lab_materialen//lab1//donker.pgm", frame);
        }
        if (key == 'j') { // *.jpg plaatje opslaan; kleur
            imwrite("C://temp//capture.jpg", frame); // The function imwrite saves the image to the specified file. The image format is chosen based on the filename extension (see cv::imread for the list of extensions).
        }
        if (key == 27) // ESC toets, dus stoppen
            return 0;
        histogram.update();
    }
    return 0;
}

int lab1_opdracht1a_beeld_spiegelen() { // 1. Spiegelen 
    Mat src, dst;

    String source_window = "Originele plaatje";         // Mag je weglaten, alleen imshow("naam",src) is voldoende. Dit maakt het mooier.
    String destination_window = "Horizontaal gespiegeld";//"Verticaal gespiegeld"; 

    /* Hieronder: plaatje inlezen met imread(). De paden moet er zo uitzien: "C://mijnMap//subMap//bestand.pgm"
    Vergeet de optie IMREAD_GRAYSCALE niet als je een grijswaardenplaatje inleest.
    Met IMREAD_GRAYSCALE krijg je 1 byte per pixel, als je het vergeet krijg je 3 bytes per pixel en dan zie je maar 1/3 van je bronplaatje terug!! */

    src = imread("C://Users//Kwik//Documents//HBO-ICT//Semester 4//Kwartaal 4//Computer Vision//lab_materialen//lab1//File1.pgm", IMREAD_GRAYSCALE);
    namedWindow(source_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(source_window, src); // Originele plaatje laten zien.
    int HEIGHT = src.rows;
    int WIDTH = src.cols;
    int temp = 0;

    dst = Mat::ones(HEIGHT, WIDTH, CV_8U) * 0;   // Maak een nieuw plaatje van hetzelfde type als src, gevuld met 1 * 0 (dus nullen, dus zwart)

    Mirror spiegel(src, dst);
    spiegel.process();

    namedWindow(destination_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(destination_window, dst); // Bestemmingsplaatje laten zien.

    // Plaatjes netjes positioneren (mag je weglaten, alleen imshow() is voldoende)
    moveWindow(source_window, 0, 0); // Plaatje naar linksboven verplaatsen
    moveWindow(destination_window, WIDTH, 0); // Plaatje rechts naast de andere plaatsen

    waitKey(0); // Wachten tot een toets gedrukt wordt...
    return 0;
}

int lab1_opdracht1b_grijswaarden_inverteren() { // 2. Zwart en wit tinten inverteren
    Mat src, dst;

    String source_window = "Originele plaatje";         // Mag je weglaten, alleen imshow("naam",src) is voldoende. Dit maakt het mooier.
    String destination_window = "Ge�nverteerd";//"Verticaal gespiegeld"; 

    /* Hieronder: plaatje inlezen met imread(). De paden moet er zo uitzien: "C://mijnMap//subMap//bestand.pgm"
    Vergeet de optie IMREAD_GRAYSCALE niet als je een grijswaardenplaatje inleest.
    Met IMREAD_GRAYSCALE krijg je 1 byte per pixel, als je het vergeet krijg je 3 bytes per pixel en dan zie je maar 1/3 van je bronplaatje terug!! */

    src = imread("C://Users//Kwik//Documents//HBO-ICT//Semester 4//Kwartaal 4//Computer Vision//lab_materialen//lab1//File1.pgm", IMREAD_GRAYSCALE);
    namedWindow(source_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(source_window, src); // Originele plaatje laten zien.
    int HEIGHT = src.rows;
    int WIDTH = src.cols;
    int temp = 0;

    dst = Mat::ones(HEIGHT, WIDTH, CV_8U) * 0;   // Maak een nieuw plaatje van hetzelfde type als src, gevuld met 1 * 0 (dus nullen, dus zwart)

    Invert invert(src, dst);
    invert.process();

    namedWindow(destination_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(destination_window, dst); // Bestemmingsplaatje laten zien.

    // Plaatjes netjes positioneren (mag je weglaten, alleen imshow() is voldoende)
    moveWindow(source_window, 0, 0); // Plaatje naar linksboven verplaatsen
    moveWindow(destination_window, WIDTH, 0); // Plaatje rechts naast de andere plaatsen

    waitKey(0); // Wachten tot een toets gedrukt wordt...
    return 0;
}

int lab1_opdracht2_contrast_stretch() { // 3. Contrast aanpassen
    Mat src, dst;

    String source_window = "Originele plaatje";         // Mag je weglaten, alleen imshow("naam",src) is voldoende. Dit maakt het mooier.
    String destination_window = "Ge�nverteerd";//"Verticaal gespiegeld"; 

    /* Hieronder: plaatje inlezen met imread(). De paden moet er zo uitzien: "C://mijnMap//subMap//bestand.pgm"
    Vergeet de optie IMREAD_GRAYSCALE niet als je een grijswaardenplaatje inleest.
    Met IMREAD_GRAYSCALE krijg je 1 byte per pixel, als je het vergeet krijg je 3 bytes per pixel en dan zie je maar 1/3 van je bronplaatje terug!! */

    src = imread("C://Users//Kwik//Documents//HBO-ICT//Semester 4//Kwartaal 4//Computer Vision//lab_materialen//lab1//Donker.pgm", IMREAD_GRAYSCALE);
    namedWindow(source_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(source_window, src); // Originele plaatje laten zien.
    int HEIGHT = src.rows;
    int WIDTH = src.cols;
    int temp = 0;

    dst = Mat::ones(HEIGHT, WIDTH, CV_8U) * 0;   // Maak een nieuw plaatje van hetzelfde type als src, gevuld met 1 * 0 (dus nullen, dus zwart)

    Contrast contrast(src, dst);
    contrast.process();

    namedWindow(destination_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(destination_window, dst); // Bestemmingsplaatje laten zien.

    // Plaatjes netjes positioneren (mag je weglaten, alleen imshow() is voldoende)
    moveWindow(source_window, 0, 0); // Plaatje naar linksboven verplaatsen
    moveWindow(destination_window, WIDTH, 0); // Plaatje rechts naast de andere plaatsen

    Histogram source_histogram("Source", src);
    Histogram destination_histogram("Destination", dst);
    
    while (1) {
        char esc = waitKey(5);
        if (esc == 27) {
            return 0;
        }
        source_histogram.update();
        destination_histogram.update();
    }
    waitKey(0); // Wachten tot een toets gedrukt wordt...
    return 0;
}

int lab2_opdracht2_zoomen() { // 4. Zoomen
    Mat src, dst;

    String source_window = "Originele plaatje";         // Mag je weglaten, alleen imshow("naam",src) is voldoende. Dit maakt het mooier.
    String destination_window = "Gezoomed";//"Verticaal gespiegeld"; 

    /* Hieronder: plaatje inlezen met imread(). De paden moet er zo uitzien: "C://mijnMap//subMap//bestand.pgm"
    Vergeet de optie IMREAD_GRAYSCALE niet als je een grijswaardenplaatje inleest.
    Met IMREAD_GRAYSCALE krijg je 1 byte per pixel, als je het vergeet krijg je 3 bytes per pixel en dan zie je maar 1/3 van je bronplaatje terug!! */

    src = imread("C://Users//Kwik//Documents//HBO-ICT//Semester 4//Kwartaal 4//Computer Vision//lab_materialen//lab2//Rijswijk.pgm", IMREAD_GRAYSCALE);
    namedWindow(source_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(source_window, src); // Originele plaatje laten zien.
    int HEIGHT = src.rows;
    int WIDTH = src.cols;
    int temp = 0;

    int x = WIDTH / 2;
    int y = HEIGHT / 2;
    float zoomzoom = 1;

    dst = Mat::ones(HEIGHT, WIDTH, CV_8U) * 255;   // Maak een nieuw plaatje van hetzelfde type als src, gevuld met 1 * 0 (dus nullen, dus zwart)

    Zoom zoomer(src, dst, zoomzoom, x, y);
    zoomer.process();

    namedWindow(destination_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(destination_window, dst); // Bestemmingsplaatje laten zien.

    // Plaatjes netjes positioneren (mag je weglaten, alleen imshow() is voldoende)
    moveWindow(source_window, 0, 0); // Plaatje naar linksboven verplaatsen
    moveWindow(destination_window, WIDTH, 0); // Plaatje rechts naast de andere plaatsen

    while (1) {
        char p = waitKey(5);
        switch (p) {
        case 27: 
            return 0;
            break;
        case 'w':
            x = x - 1;
            break;
        case 's':
            x = x + 1;
            break;
        case 'a':
            y = y - 1;
            break;
        case 'd':
            y = y + 1;
            break;
        case '+':
            zoomzoom = zoomzoom + 1;
            cout << zoomzoom << endl;
            if (zoomzoom == 0) {
                zoomzoom = 1;
            }
            break;
        case '-':
            zoomzoom = zoomzoom - 1;
            if (zoomzoom == 0) {
                zoomzoom = -1;
            }
            cout << zoomzoom << endl;
            break;
        case '*':
            zoomzoom = 1;
            break;
        case '/':
            x = WIDTH / 2;
            y = HEIGHT / 2;
        }

        dst = Mat::ones(HEIGHT, WIDTH, CV_8U) * 255;   // Maak een nieuw plaatje van hetzelfde type als src, gevuld met 1 * 0 (dus nullen, dus zwart)
        Zoom zoomer(src, dst, zoomzoom, x, y);
        zoomer.process();

        namedWindow(destination_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
        imshow(destination_window, dst); // Bestemmingsplaatje laten zien.
    }

    waitKey(0); // Wachten tot een toets gedrukt wordt...
    return 0;
}

int lab2_opdracht2b_roteren() { // 5. Roteren
    Mat src, dst;

    String source_window = "Originele plaatje";         // Mag je weglaten, alleen imshow("naam",src) is voldoende. Dit maakt het mooier.
    String destination_window = "Gedraaid";//"Verticaal gespiegeld"; 

    /* Hieronder: plaatje inlezen met imread(). De paden moet er zo uitzien: "C://mijnMap//subMap//bestand.pgm"
    Vergeet de optie IMREAD_GRAYSCALE niet als je een grijswaardenplaatje inleest.
    Met IMREAD_GRAYSCALE krijg je 1 byte per pixel, als je het vergeet krijg je 3 bytes per pixel en dan zie je maar 1/3 van je bronplaatje terug!! */

    src = imread("C://Users//Kwik//Documents//HBO-ICT//Semester 4//Kwartaal 4//Computer Vision//lab_materialen//lab2//Rijswijk.pgm", IMREAD_GRAYSCALE);
    namedWindow(source_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(source_window, src); // Originele plaatje laten zien.
    int HEIGHT = src.rows;
    int WIDTH = src.cols;
    int temp = 0;
    float hoek = 45;

    dst = Mat::ones(HEIGHT, WIDTH, CV_8U) * 255;   // Maak een nieuw plaatje van hetzelfde type als src, gevuld met 1 * 0 (dus nullen, dus zwart)

    Rotate rotator(src, dst, hoek);
    rotator.process();

    namedWindow(destination_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(destination_window, dst); // Bestemmingsplaatje laten zien.

    // Plaatjes netjes positioneren (mag je weglaten, alleen imshow() is voldoende)
    moveWindow(source_window, 0, 0); // Plaatje naar linksboven verplaatsen
    moveWindow(destination_window, WIDTH, 0); // Plaatje rechts naast de andere plaatsen

    while (1) {
        char p = waitKey(5);
        switch (p) {
        case 27:
            return 0;
            break;
        case '+':
            hoek = hoek + 1;
            cout << hoek << endl;
            cout << cos(hoek) << endl;
            break;
        case '-':
            hoek = hoek - 1;
            cout << hoek << endl;
            break;
        case '*':
            hoek = 0;
            break;
        }

        dst = Mat::ones(HEIGHT, WIDTH, CV_8U) * 255;   // Maak een nieuw plaatje van hetzelfde type als src, gevuld met 1 * 0 (dus nullen, dus zwart)
        Rotate rotator(src, dst, hoek);
        rotator.process();

        namedWindow(destination_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
        imshow(destination_window, dst); // Bestemmingsplaatje laten zien.
    }

    waitKey(0); // Wachten tot een toets gedrukt wordt...
    return 0;
}

int lab2_opdracht3_affien_mysterie() { // 6. Mysterieuze affiene transformatie
    Mat src, dst;

    String source_window = "Originele plaatje";         // Mag je weglaten, alleen imshow("naam",src) is voldoende. Dit maakt het mooier.
    String destination_window = "Getransformeerd";//"Verticaal gespiegeld"; 

    /* Hieronder: plaatje inlezen met imread(). De paden moet er zo uitzien: "C://mijnMap//subMap//bestand.pgm"
    Vergeet de optie IMREAD_GRAYSCALE niet als je een grijswaardenplaatje inleest.
    Met IMREAD_GRAYSCALE krijg je 1 byte per pixel, als je het vergeet krijg je 3 bytes per pixel en dan zie je maar 1/3 van je bronplaatje terug!! */

    src = imread("C://Users//Kwik//Documents//HBO-ICT//Semester 4//Kwartaal 4//Computer Vision//lab_materialen//lab2//Rijswijk.pgm", IMREAD_GRAYSCALE);
    namedWindow(source_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(source_window, src); // Originele plaatje laten zien.
    int HEIGHT = src.rows;
    int WIDTH = src.cols;
    int temp = 0;

    dst = Mat::ones(HEIGHT, WIDTH, CV_8U) * 255;   // Maak een nieuw plaatje van hetzelfde type als src, gevuld met 1 * 0 (dus nullen, dus zwart)

    VerdelingEnContrast contrast(src, dst);
    contrast.process();

    namedWindow(destination_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(destination_window, dst); // Bestemmingsplaatje laten zien.

    // Plaatjes netjes positioneren (mag je weglaten, alleen imshow() is voldoende)
    moveWindow(source_window, 0, 0); // Plaatje naar linksboven verplaatsen
    moveWindow(destination_window, WIDTH, 0); // Plaatje rechts naast de andere plaatsen
    waitKey(0); // Wachten tot een toets gedrukt wordt...
    return 0;
}

int lab3_opdracht1_hoogdoorlaat() { // 7. Hoogdoorlaat filter
    Mat src, dst;

    String source_window = "Originele plaatje";         // Mag je weglaten, alleen imshow("naam",src) is voldoende. Dit maakt het mooier.
    String destination_window = "Hoogdoorlaat filter";//"Verticaal gespiegeld"; 

    /* Hieronder: plaatje inlezen met imread(). De paden moet er zo uitzien: "C://mijnMap//subMap//bestand.pgm"
    Vergeet de optie IMREAD_GRAYSCALE niet als je een grijswaardenplaatje inleest.
    Met IMREAD_GRAYSCALE krijg je 1 byte per pixel, als je het vergeet krijg je 3 bytes per pixel en dan zie je maar 1/3 van je bronplaatje terug!! */

    src = imread("C://Users//Kwik//Documents//HBO-ICT//Semester 4//Kwartaal 4//Computer Vision//lab_materialen//lab3//joint.jpg", IMREAD_GRAYSCALE);
    namedWindow(source_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(source_window, src); // Originele plaatje laten zien.
    int HEIGHT = src.rows;
    int WIDTH = src.cols;
    int temp = 0;

    dst = Mat::ones(HEIGHT, WIDTH, CV_8U) * 0;   // Maak een nieuw plaatje van hetzelfde type als src, gevuld met 1 * 0 (dus nullen, dus zwart)

    Filter filter(src, dst);
    filter.hoogdoorlaat();

    namedWindow(destination_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(destination_window, dst); // Bestemmingsplaatje laten zien.

    // Plaatjes netjes positioneren (mag je weglaten, alleen imshow() is voldoende)
    moveWindow(source_window, 0, 0); // Plaatje naar linksboven verplaatsen
    moveWindow(destination_window, WIDTH, 0); // Plaatje rechts naast de andere plaatsen

    waitKey(0); // Wachten tot een toets gedrukt wordt...
    return 0;
}

int lab3_opdracht2a_laagdoorlaat() { // 8. Laagdoorlaat filter
    Mat src, dst;

    String source_window = "Originele plaatje";         // Mag je weglaten, alleen imshow("naam",src) is voldoende. Dit maakt het mooier.
    String destination_window = "Laagdoorlaat filter";//"Verticaal gespiegeld"; 

    /* Hieronder: plaatje inlezen met imread(). De paden moet er zo uitzien: "C://mijnMap//subMap//bestand.pgm"
    Vergeet de optie IMREAD_GRAYSCALE niet als je een grijswaardenplaatje inleest.
    Met IMREAD_GRAYSCALE krijg je 1 byte per pixel, als je het vergeet krijg je 3 bytes per pixel en dan zie je maar 1/3 van je bronplaatje terug!! */

    src = imread("C://Users//Kwik//Documents//HBO-ICT//Semester 4//Kwartaal 4//Computer Vision//lab_materialen//lab3//whalesp.jpg", IMREAD_GRAYSCALE);
    namedWindow(source_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(source_window, src); // Originele plaatje laten zien.
    int HEIGHT = src.rows;
    int WIDTH = src.cols;
    int temp = 0;

    dst = Mat::ones(HEIGHT, WIDTH, CV_8U) * 0;   // Maak een nieuw plaatje van hetzelfde type als src, gevuld met 1 * 0 (dus nullen, dus zwart)

    Filter filter(src, dst);
    filter.laagdoorlaat();

    namedWindow(destination_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(destination_window, dst); // Bestemmingsplaatje laten zien.

    // Plaatjes netjes positioneren (mag je weglaten, alleen imshow() is voldoende)
    moveWindow(source_window, 0, 0); // Plaatje naar linksboven verplaatsen
    moveWindow(destination_window, WIDTH, 0); // Plaatje rechts naast de andere plaatsen

    waitKey(0); // Wachten tot een toets gedrukt wordt...
    return 0;
}

int lab3_opdracht2b_mediaan() { // 9. Mediaan filter
    Mat src, dst;

    String source_window = "Originele plaatje";         // Mag je weglaten, alleen imshow("naam",src) is voldoende. Dit maakt het mooier.
    String destination_window = "Mediaan filter";//"Verticaal gespiegeld"; 

    /* Hieronder: plaatje inlezen met imread(). De paden moet er zo uitzien: "C://mijnMap//subMap//bestand.pgm"
    Vergeet de optie IMREAD_GRAYSCALE niet als je een grijswaardenplaatje inleest.
    Met IMREAD_GRAYSCALE krijg je 1 byte per pixel, als je het vergeet krijg je 3 bytes per pixel en dan zie je maar 1/3 van je bronplaatje terug!! */

    src = imread("C://Users//Kwik//Documents//HBO-ICT//Semester 4//Kwartaal 4//Computer Vision//lab_materialen//lab3//whalesp.jpg", IMREAD_GRAYSCALE);
    namedWindow(source_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(source_window, src); // Originele plaatje laten zien.
    int HEIGHT = src.rows;
    int WIDTH = src.cols;
    int temp = 0;

    dst = Mat::ones(HEIGHT, WIDTH, CV_8U) * 0;   // Maak een nieuw plaatje van hetzelfde type als src, gevuld met 1 * 0 (dus nullen, dus zwart)

    Filter filter(src, dst);
    filter.mediaan();

    namedWindow(destination_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(destination_window, dst); // Bestemmingsplaatje laten zien.

    // Plaatjes netjes positioneren (mag je weglaten, alleen imshow() is voldoende)
    moveWindow(source_window, 0, 0); // Plaatje naar linksboven verplaatsen
    moveWindow(destination_window, WIDTH, 0); // Plaatje rechts naast de andere plaatsen

    waitKey(0); // Wachten tot een toets gedrukt wordt...
    return 0;
}

int lab4_opdracht3_mieren() { // 10. Mier(en)
    Mat src1, src2, dst;

    String source_window1 = "Originele plaatje 1";         // Mag je weglaten, alleen imshow("naam",src) is voldoende. Dit maakt het mooier.
    string source_window2 = "Originele plaatje 2";
    String destination_window = "Resultaat";//"Verticaal gespiegeld"; 

    /* Hieronder: plaatje inlezen met imread(). De paden moet er zo uitzien: "C://mijnMap//subMap//bestand.pgm"
    Vergeet de optie IMREAD_GRAYSCALE niet als je een grijswaardenplaatje inleest.
    Met IMREAD_GRAYSCALE krijg je 1 byte per pixel, als je het vergeet krijg je 3 bytes per pixel en dan zie je maar 1/3 van je bronplaatje terug!! */

    src1 = imread("C://Users//Kwik//Documents//HBO-ICT//Semester 4//Kwartaal 4//Computer Vision//lab_materialen//lab4//ants1.1.pgm", IMREAD_GRAYSCALE);
    namedWindow(source_window1, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(source_window1, src1); // Originele plaatje laten zien.

    src2 = imread("C://Users//Kwik//Documents//HBO-ICT//Semester 4//Kwartaal 4//Computer Vision//lab_materialen//lab4//ants2.2.pgm", IMREAD_GRAYSCALE);
    namedWindow(source_window2, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(source_window2, src2); // Originele plaatje laten zien.

    int HEIGHT1 = src1.rows;
    int WIDTH1 = src1.cols;
    int temp = 0;

    dst = Mat::ones(HEIGHT1, WIDTH1, CV_8U) * 0;   // Maak een nieuw plaatje van hetzelfde type als src, gevuld met 1 * 0 (dus nullen, dus zwart)

    Mier mier(src1, src2, dst);
    mier.process();

    namedWindow(destination_window, WINDOW_AUTOSIZE); // Mag je weglaten, alleen imshow("naam",src) is voldoende.
    imshow(destination_window, dst); // Bestemmingsplaatje laten zien.

    // Plaatjes netjes positioneren (mag je weglaten, alleen imshow() is voldoende)
    moveWindow(source_window1, 0, 0); // Plaatje naar linksboven verplaatsen
    moveWindow(source_window2, WIDTH1, 0); // Plaatje naar linksboven verplaatsen
    moveWindow(destination_window, 0, HEIGHT1); // Plaatje rechts naast de andere plaatsen

    waitKey(0); // Wachten tot een toets gedrukt wordt...
    return 0;
}

int lab5_opdracht1_gaten_tellen() { // 11. Gaten tellen1
    return 0;
}

int lab5_opdracht2_labellen() { // 12. Gaten tellen2 - verbeterd
    return 0;
}

int main(int argc, const char** argv){

    int optie, ret;
    do {
        system("Color 0A"); // groene tekst (A) op zwarte achtergrond (0)
        //system("Color %"); // laat alle mogelijke kleuren zien
        cout << "H-NSE-IMVIS   -   Maak een keuze (geef nummer + druk Enter)" << endl;
        cout << "-1: Demo met camera en histogram" << endl;
        cout << " 1: Lab 1 Opdracht 1a - Spiegelen" << endl;
        cout << " 2: Lab 1 Opdracht 1b - Inverteren" << endl;
        cout << " 3: Lab 1 Opdracht 2  - Contrast aanpassen" << endl;
        cout << " 4: Lab 2 Opdracht 2  - Affien: Zoomen" << endl;
        cout << " 5: Lab 2 Opdracht 2b - Affien: Roteren" << endl;
        cout << " 6: Lab 2 Opdracht 3  - Mysterieuze affiene transformatie - LET OP! : TOETSVRAGEN!" << endl;
        cout << " 7: Lab 3 Opdracht 1  - Hoogdoorlaat filter" << endl;
        cout << " 8: Lab 3 Opdracht 2a - Laagdoorlaat filter" << endl;
        cout << " 9: Lab 3 Opdracht 2b - Mediaan filter" << endl;
        cout << "10: Lab 4 Opdracht 3  - Mier(en)" << endl;
        cout << "11: Lab 5 Opdracht 1  - Gaten tellen1" << endl;
        cout << "12: Lab 5 Opdracht 2  - Gaten tellen2 - verbeterd" << endl;
        cout << endl << "Maak een keuze (0 = stoppen): ";
        cin >> optie;

        switch (optie) {
        case -1: ret = demo_met_camera(); break;
        case 0: ret = 0; break;
        case 1: ret = lab1_opdracht1a_beeld_spiegelen(); break;
        case 2: ret = lab1_opdracht1b_grijswaarden_inverteren(); break;
        case 3: ret = lab1_opdracht2_contrast_stretch(); break;
        case 4: ret = lab2_opdracht2_zoomen(); break;
        case 5: ret = lab2_opdracht2b_roteren(); break;
        case 6: ret = lab2_opdracht3_affien_mysterie(); break;
        case 7: ret = lab3_opdracht1_hoogdoorlaat(); break;
        case 8: ret = lab3_opdracht2a_laagdoorlaat(); break;
        case 9: ret = lab3_opdracht2b_mediaan(); break;
        case 10: ret = lab4_opdracht3_mieren(); break;
        case 11: ret = lab5_opdracht1_gaten_tellen(); break;
        case 12: ret = lab5_opdracht2_labellen(); break;
        }

        if (ret != 0) return ret;
        destroyAllWindows();    // Alle openstaande imshow() vensters sluiten
    } while (optie != 0);
    return 0;
}

