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
                                  Display display, int pixel, int offsetX,
                                  int offsetY) {
    int size = qrcodegen_getSize(qr);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (qrcodegen_getModule(qr, x, y)) {
                // Serial.print("*");
                display.setColor(EPD_BLACK);
                display.fillRectangle(x * pixel + offsetX, y * pixel + offsetY,
                                      pixel, pixel);
            } else {
                // Serial.print(" ");
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
    for (int i = 0; i <= entryDataList.size() - 1; i++) {
        data += entryDataList.get(i).toString();
    }

    return device->id + IdentifierGenerator::generateUniqueNumberId(32) +
           device->errorsHandler->toString() + data;
}