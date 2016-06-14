/*
 *  Copyright 2010-2011 Alessandro Francescon
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>
#include <exception>
#include <stdlib.h>
#include <zxing/common/Counted.h>
#include <zxing/Binarizer.h>
#include <zxing/MultiFormatReader.h>
#include <zxing/Result.h>
#include <zxing/ReaderException.h>
#include <zxing/common/GlobalHistogramBinarizer.h>
#include <zxing/Exception.h>
#include <zxing/common/IllegalArgumentException.h>
#include <zxing/BinaryBitmap.h>
#include <zxing/DecodeHints.h>
#include <zxing/qrcode/QRCodeReader.h>
#include <zxing/MultiFormatReader.h>
#include <zxing/MatSource.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace zxing;
using namespace zxing::qrcode;
using namespace cv;

void printUsage(char** argv) {

    // Print usage
    cout << "Usage: " << argv[0] << " [-d <DEVICE>] [-w <CAPTUREWIDTH>] [-h <CAPTUREHEIGHT>]" << endl
         << "Read QR code from given video device." << endl
         << endl;

}

Point toCvPoint(Ref<ResultPoint> resultPoint) {
    return Point(resultPoint->getX(), resultPoint->getY());
}

int main(int argc, char** argv) {

    int deviceId = 0;
    int captureWidth = 640;
    int captureHeight = 480;
    bool multi = false;

    for (int j = 0; j < argc; j++) {

        // Get arg
        string arg = argv[j];

        if (arg.compare("-d") == 0) {

            if ((j + 1) < argc) {

                // Set device id
                deviceId = atoi(argv[++j]);

            } else {

                // Log
                cerr << "Missing device id after -d" << endl;
                printUsage(argv);
                return 1;

            }

        } else if (arg.compare("-w") == 0) {

            if ((j + 1) < argc) {

                // Set capture width
                captureWidth = atoi(argv[++j]);

            } else {

                // Log
                cerr << "Missing width after -w" << endl;
                printUsage(argv);
                return 1;

            }

        } else if (arg.compare("-h") == 0) {

            if ((j + 1) < argc) {

                // Set capture height
                captureHeight = atoi(argv[++j]);

            } else {

                // Log
                cerr << "Missing height after -h" << endl;
                printUsage(argv);
                return 1;

            }

        } else if (arg.compare("-m") == 0) {

            // Set multi to true
            multi = true;

        }

    }

    // Log
    cout << "Capturing from device " << deviceId << "..." << endl;

    // Open video captire
    VideoCapture videoCapture(deviceId);

    if (!videoCapture.isOpened()) {

        // Log
        cerr << "Open video capture failed on device id: " << deviceId << endl;
        return -1;

    }

    if (!videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, captureWidth)) {

        // Log
        cerr << "Failed to set frame width: " << captureWidth << " (ignoring)" << endl;

    }

    if (!videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, captureHeight)) {

        // Log
        cerr << "Failed to set frame height: " << captureHeight << " (ignoring)" << endl;

    }

    // The captured image and its grey conversion
    Mat image, grey;

    // Open output window
    namedWindow("ZXing", cv::WINDOW_AUTOSIZE);

    // Stopped flag will be set to -1 from subsequent wayKey() if no key was pressed
    int stopped = -1;

    while (stopped == -1) {

        // Capture image
        bool result = videoCapture.read(image);

        if (result) {

            // Convert to grayscale
            cvtColor(image, grey, CV_BGR2GRAY);

            try {

                // Create luminance  source
                Ref<LuminanceSource> source = MatSource::create(grey);

                // Search for QR code
                Ref<Reader> reader;

                if (multi) {
                    reader.reset(new MultiFormatReader);
                } else {
                    reader.reset(new QRCodeReader);
                }

                Ref<Binarizer> binarizer(new GlobalHistogramBinarizer(source));
                Ref<BinaryBitmap> bitmap(new BinaryBitmap(binarizer));
                Ref<Result> result(reader->decode(bitmap, DecodeHints(DecodeHints::TRYHARDER_HINT)));

                // Get result point count
                int resultPointCount = result->getResultPoints()->size();

                for (int j = 0; j < resultPointCount; j++) {

                    // Draw circle
                    circle(image, toCvPoint(result->getResultPoints()[j]), 10, Scalar( 110, 220, 0 ), 2);

                }

                // Draw boundary on image
                if (resultPointCount > 1) {

                    for (int j = 0; j < resultPointCount; j++) {

                        // Get start result point
                        Ref<ResultPoint> previousResultPoint = (j > 0) ? result->getResultPoints()[j - 1] : result->getResultPoints()[resultPointCount - 1];

                        // Draw line
                        line(image, toCvPoint(previousResultPoint), toCvPoint(result->getResultPoints()[j]), Scalar( 110, 220, 0 ),  2, 8 );

                        // Update previous point
                        previousResultPoint = result->getResultPoints()[j];

                    }

                }

                if (resultPointCount > 0) {

                    // Draw text
                    putText(image, result->getText()->getText(), toCvPoint(result->getResultPoints()[0]), FONT_HERSHEY_PLAIN, 1, Scalar( 110, 220, 0 ));

                }

            } catch (const ReaderException& e) {
                cerr << e.what() << " (ignoring)" << endl;
            } catch (const zxing::IllegalArgumentException& e) {
                cerr << e.what() << " (ignoring)" << endl;
            } catch (const zxing::Exception& e) {
                cerr << e.what() << " (ignoring)" << endl;
            } catch (const std::exception& e) {
                cerr << e.what() << " (ignoring)" << endl;
            }

            // Show captured image
            imshow("ZXing", image);

            // Wait a key for 1 millis
            stopped = waitKey(1);

        } else {

            // Log
            cerr << "video capture failed" << endl;

        }

    }

    // Release video capture
    videoCapture.release();

    return 0;

}
