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

#include "MatSource.h"
#include <zxing/common/IllegalArgumentException.h>

zxing::Ref<zxing::LuminanceSource> MatSource::create(cv::Mat & cvImage) {
    return zxing::Ref<LuminanceSource>(new MatSource(cvImage));
}

MatSource::MatSource(cv::Mat & _cvImage) : zxing::LuminanceSource(_cvImage.cols, _cvImage.rows) {
    cvImage = _cvImage.clone();
}

zxing::ArrayRef<char> MatSource::getRow(int y, zxing::ArrayRef<char> row) const {

    // Get width
    int width = getWidth();

    if (!row) {

        // Create row
        row = zxing::ArrayRef<char>(width);

    }

    // Get pointer to row
    const char *p = cvImage.ptr<char>(y);

    for(int x = 0; x < width; ++x, ++p) {

        // Set row at index x
        row[x] = *p;

    }

    return row;

}

zxing::ArrayRef<char> MatSource::getMatrix() const {

    // Get width and height
    int width = getWidth();
    int height =  getHeight();

    // Create matrix
    zxing::ArrayRef<char> matrix = zxing::ArrayRef<char>(width * height);

    for (int y = 0; y < height; ++y) {

        // Get pointer to row
        const char *p = cvImage.ptr<char>(y);

        // Calculate y offset
        int yoffset = y * width;

        for(int x = 0; x < width; ++x, ++p) {

            // Set row at index x with y offset
            matrix[yoffset + x] = *p;

        }

    }

    return matrix;

}
