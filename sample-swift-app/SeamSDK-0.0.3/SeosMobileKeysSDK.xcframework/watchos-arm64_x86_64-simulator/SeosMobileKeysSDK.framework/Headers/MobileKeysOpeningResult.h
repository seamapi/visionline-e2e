// MobileKeysOpeningResult.h
// Copyright (c) 2014 ASSA ABLOY Mobile Services ( http://assaabloy.com/seos )
//
// All rights reserved.

#import <Foundation/Foundation.h>
#import "MobileKeysReader.h"

/**
 * This class represents the result of a reader opening.
 */
@interface MobileKeysOpeningResult : NSObject

/**
 * Status of the connection
 */
@property(nonatomic) MobileKeysOpeningStatusType status;

/**
 * ConnectionDuration is the time interval when phone and reader communicates over some media (BLE, ...)
 */
@property(nonatomic) NSTimeInterval connectionDuration;

/**
 * Opening status payload. EOT fragmend body. Application specific data that was returned from the lock.
 *
* @return additional payload related to the opening, default is nil.
 */
@property(nonatomic) NSData *statusPayload;

/**
 * Initilzer, which constructs `MobileKeysOpeningResult` object with given `MobileKeysOpeningStatusType` and payload
 *
 * @return instance of `MobileKeysOpeningResult`.
 */
- (instancetype)initWithStatus:(MobileKeysOpeningStatusType)status statusPayload:(NSData *)statusPayload;

/**
 * Returns a readable description about the `MobileKeysOpeningResult` object
 *
 * @return String description of the object.
 */
- (NSString *)description;

@end
