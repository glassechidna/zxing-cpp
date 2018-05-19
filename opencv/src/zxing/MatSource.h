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

#ifndef __MAT_SOURCE_H_
#define __MAT_SOURCE_H_

#include <zxing/LuminanceSource.h>
#include <opencv2/core/core.hpp>

class MatSource : public zxing::LuminanceSource {
private:
    cv::Mat cvImage;

public:

    static zxing::Ref<zxing::LuminanceSource> create(cv::Mat & _cvImage);

    MatSource(cv::Mat & _cvImage);

    zxing::ArrayRef<char> getRow(int y, zxing::ArrayRef<char> row) const;
    zxing::ArrayRef<char> getMatrix() const;

    bool isCropSupported() const override;

    zxing::Ref<LuminanceSource> crop(int left, int top, int width, int height) const override;

};

#endif /* __MAT_SOURCE_H_ */
