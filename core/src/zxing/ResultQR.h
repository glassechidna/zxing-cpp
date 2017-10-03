#ifndef __RESULTQR_H__
#define __RESULTQR_H__
//  ResultQR.h
//  FingerprintReader
//
//  Created by ScanTrust on 17/02/14.
//  Copyright (c) 2014 ScanTrust. All rights reserved.
//

#include <zxing/common/Counted.h>
#include <zxing/Result.h>

namespace zxing {
    
    class ResultQR : public Counted {
    private:
        Ref<Result> result_;
        int qrSize_;
        
    public:
        ResultQR(Ref<Result> result, int qrSize);
        ~ResultQR();
        int getSize();
        Ref<Result> getResult();
        
    };
}

#endif //__RESULTQR_H__
