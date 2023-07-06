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

#define S_TO_HOUR_FACTOR 3600
#define HOUR_COUNT_LENGTH 3

/* Qr Code Handler class definition */
class QrCodeHandler {
   public:
    /**
     * @brief Function that prints a qrcode
     * @param batteryVoltage : Voltage of the battery
     * @return Battery error as level (HIGH, HALF, LOW)
     */
    static QrCodeError generateQrCode(
        String qrText, uint8_t qr[qrcodegen_BUFFER_LEN_MAX],
        uint8_t version = 32, qrcodegen_Ecc correction = qrcodegen_Ecc_MEDIUM);

    /**
     * @brief Function that prints a qrcode
     * @param batteryVoltage : Voltage of the battery
     * @return Battery error as level (HIGH, HALF, LOW)
     */
    static void displayQrCode(uint8_t qr[qrcodegen_BUFFER_LEN_MAX],
                              Display display, int pixel = 2, int offsetX = 5,
                              int offsetY = 105);

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
    /**
     * @brief Function that format hours to string adding 0 as padding based on wanted length
     * @param hours  : Hours to convert as string
     * @param length : Length of wanted string
     * @return String of hours and padding 0 if needed
     */
    static String formatHoursToString(int hours, int length);
};

#endif
