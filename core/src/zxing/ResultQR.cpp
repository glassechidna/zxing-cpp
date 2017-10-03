// -*- mode:c++; tab-width:2; indent-tabs-mode:nil; c-basic-offset:2 -*-

//  ResultQR.cpp
//  FingerprintReader
//
//  Created by ScanTrust on 17/02/14.
//  Copyright (c) 2014 ScanTrust. All rights reserved.
//

#include <zxing/ResultQR.h>


using zxing::ResultQR;
using zxing::Result;
using zxing::Ref;


ResultQR::ResultQR(Ref<Result> result, int qrSeize) : result_(result), qrSize_(qrSeize) {
    
}

ResultQR::~ResultQR() {
    
}

Ref<Result> ResultQR::getResult() {
    return result_;
}

int ResultQR::getSize() {
    return qrSize_;
}
