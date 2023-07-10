#ifndef __QR_CODE_HANLDER__
#define __QR_CODE_HANLDER__

#include "Arduino.h"
#include "IdentifierGenerator.h"
#include "LinkedList.h"
#include "devices/Display.h"
#include "errors.h"
#include "model/Device.h"
#include "model/EntryData.h"
#include "qrcodegen.h"
#include "utility/Logger.h"
#include "utility/StringFormatter.h"

#define S_TO_HOUR_FACTOR 3600
#define HOUR_COUNT_LENGTH 3

/* QR Code Handler class definition */
class QrCodeHandler {
   public:
    /**
     * @brief Function that generates a QR code
     * @param qrText     : Text to be written
     * @param qr         : Array of bytes
     * @param version    : Version of the QR code
     * @param correction : Error correction of the QR code
     * @return Qr code error if any
     */
    static QrCodeError generateQrCode(
        String qrText, uint8_t qr[qrcodegen_BUFFER_LEN_MAX],
        uint8_t version = 32, qrcodegen_Ecc correction = qrcodegen_Ecc_MEDIUM);

    /**
     * @brief Function that prints a QR code
     * @param qr      : Array of bytes
     * @param display : Display to draw
     * @param offsetX : Offset X
     * @param offsetY : Offset Y
     * @param pixel   : Dimension of single rectangle
     */
    static void displayQrCode(uint8_t qr[qrcodegen_BUFFER_LEN_MAX],
                              Display display, int offsetX,
                              int offsetY, int pixel = 2);

    /**
     * @brief Function that generates the string for QR code from data stored in
     * File
     * @param device : Device object
     * @param entryDataList : List of entry data used to generate the string
     * @return String of all collected data
     */
    static String generateStringForQr(Device* device,
                                      LinkedList<EntryData> entryDataList);

   private:
};

#endif
