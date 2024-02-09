// MobileKeysApduResponse.h
// Copyright (c) 2016 ASSA ABLOY Mobile Services ( http://assaabloy.com/seos )
//
// All rights reserved.

#import <Foundation/Foundation.h>

/**
 * Status words returned from the application (seos) running on the secure element.
 */
typedef NS_ENUM(NSInteger, SeosStatusWord) {
    /** No error or Success. */
    APDU_STATUS_WORD_NO_ERROR = 0x9000,
    /** Applet selection failed */
    APDU_STATUS_WORD_APPLET_SELECT_FAILED = 0x6999,
    /** Normal Processing; SW2 encodes the number of data bytes still available */
    APDU_STATUS_WORD_BYTES_REMAINING = 0x6100,
    /** Class not supported */
    APDU_STATUS_WORD_CLA_NOT_SUPPORTED = 0x6e00,
    /** Command not allowed */
    APDU_STATUS_WORD_COMMAND_NOT_ALLOWED = 0x6986,
    /** Conditions of use not satisfied */
    APDU_STATUS_WORD_CONDITIONS_NOT_SATISFIED = 0x6985,
    /** Incorrect Length */
    APDU_STATUS_WORD_CORRECT_LENGTH = 0x6c00,
    /** Data part is invalid */
    APDU_STATUS_WORD_DATA_INVALID = 0x6984,
    /** Not enough memory space in the file */
    APDU_STATUS_WORD_FILE_FULL = 0x6A84,
    /** File invalid */
    APDU_STATUS_WORD_FILE_INVALID = 0x6983,
    /** File or application not found */
    APDU_STATUS_WORD_FILE_NOT_FOUND = 0x6A82,
    /** Function not supported */
    APDU_STATUS_WORD_FUNC_NOT_SUPPORTED = 0x6A81,
    /** Incorrect parameters P1-P2 */
    APDU_STATUS_WORD_INCORRECT_P1P2 = 0x6A86,
    /** Instruction code not supported or invalid */
    APDU_STATUS_WORD_INS_NOT_SUPPORTED = 0x6D00,
    /** Record not found */
    APDU_STATUS_WORD_RECORD_NOT_FOUND = 0x6A83,
    /** Security status not satisfied */
    APDU_STATUS_WORD_SECURITY_STATUS_NOT_SATISFIED = 0x6982,
    /** Incorrect secure messaging data objects */
    APDU_STATUS_WORD_SECURE_MESSAGING_INCORRECT = 0x6988,
    /** No precise diagnosis */
    APDU_STATUS_WORD_UNKNOWN = 0x6F00,
    /** Incorrect parameters in the command data field */
    APDU_STATUS_WORD_WRONG_DATA = 0x6A80,
    /** Wrong length; no further indication */
    APDU_STATUS_WORD_WRONG_LENGTH = 0x6700,
    /** Wrong P1 / P2 value */
    APDU_STATUS_WORD_WRONG_P1P2 = 0x6B00
};

/**
 * Holds the response from a APDU command. A response consists of a mandatory StatusWord and optional
 * response data.
 *
 * @see <a href="http://en.wikipedia.org/wiki/Smart_card_application_protocol_data_unit">APDU format overview on Wikipedia</a>
 * @note since version 5.0.0
 */
@interface MobileKeysApduResponse : NSObject

/**
 * Use `getData` to get the APDU data
 */
@property(nonatomic, strong) NSData *data;

/**
 * Use `getStatusWord` to get the proper `SeosStatusWord`
 */
@property(nonatomic, strong) NSData *statusWordData;

/**
 * Constructs a APDU response from a byte array.
 *
 * @param data The result from executing a APDU command.
 */
- (instancetype)initWithRawData:(NSData *)data;

/**
 * Constructs a APDU response from a StatusWord and an optional payload
 * @param payload optional payload
 * @param statusWordData required StatusWord
 */
- (instancetype)initWithData:(NSData *)payload statusWord:(NSData *)statusWordData;

/**
 * Returns data and status word 
 */
- (NSData *)toBytes;

/**
 * Returns YES if status word returns not ok and there is no more data
 */
- (BOOL)isError;

/**
 * Get status word from apdu response
 */
- (SeosStatusWord)getStatusWord;

/**
 * Returns YES if status word returns is `APDU_STATUS_WORD_NO_ERROR`
 */
- (BOOL) isOk;

/**
 * Returns YES if status word returns more data
 */
- (BOOL) isMoreData;

/**
 * Get data from APDU response or `nil` if there was no data.
 */
- (NSData *)getData;

/**
 * Returns the status word as NSData
 */
- (NSData *)status;

/**
 * If the response StatusWord was inside the `0x61XX` / `APDU_STATUS_WORD_BYTES_REMAINING` range this will indicate
 * how much data there is left to process.
 */
- (unsigned char *)getExpectedLengthOfNextResponse;

@end
