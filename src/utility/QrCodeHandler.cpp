#include "QrCodeHandler.h"

QrCodeError QrCodeHandler::generateQrCode(String qrText,
                                          uint8_t qr[qrcodegen_BUFFER_LEN_MAX],
                                          uint8_t version,
                                          qrcodegen_Ecc correction) {
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];

    bool result =
        qrcodegen_encodeText(qrText.c_str(), tempBuffer, qr, correction,
                             version, version, qrcodegen_Mask_AUTO, true);

    return (result) ? QR_OK : QR_CANT_CREATE;
}

void QrCodeHandler::displayQrCode(uint8_t qr[qrcodegen_BUFFER_LEN_MAX],
                                  Display display, int offsetX, int offsetY,
                                  int pixel) {
    int size = qrcodegen_getSize(qr);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (qrcodegen_getModule(qr, x, y)) {
                display.setColor(EPD_BLACK);
                display.fillRectangle(x * pixel + offsetX, y * pixel + offsetY,
                                      pixel, pixel);
            } else {
                display.setColor(EPD_WHITE);
                display.fillRectangle(x * pixel + offsetX, y * pixel + offsetY,
                                      pixel, pixel);
            }
        }
    }
}

String QrCodeHandler::generateStringForQr(Device* device,
                                          LinkedList<EntryData> entryDataList) {
    String data = "";
    data += entryDataList.get(0).timestamp;
    for (int i = 0; i <= entryDataList.size() - 1; i++) {
        data += StringFormatter::formatNumberToStringWithSign(
            round((double)(entryDataList.get(i).timestamp -
                           entryDataList.get(0).timestamp) /
                  S_TO_HOUR_FACTOR),
            HOUR_COUNT_LENGTH);
        data += entryDataList.get(i).envData.toString();
    }

    return device->id + IdentifierGenerator::generateUniqueNumberId(32) +
           device->errorsData->toString() + data;
}