#ifndef __CORE__
#define __CORE__
#include "stdint.h"
#include "stdbool.h"
#include "st7789.h"
#include "eeprom.h"
#include "HelpingModule.h"
// COLOR DEFENITION
#define DEFAULT_FONT_COLOR BLACK
#define BACKGROUND_COLOR WHITE
#define BG_CURRENT_STATE_COLOR DARKBLUE
#define CURRENT_STATE_COLOR WHITE
#define CURRENT_NUMBER_COLOR GREEN
#define CURRENT_PROFILE GREEN
#define ERROR_DEFAULT_FONT_COLOR RED
#define DEFAULT_FONT_COLOR BLACK
#define EXT_DEFAULT_FONT_COLOR GRAY

// ARRAY_TO_NUMBER DEFENITION
#define HZRequestNum ((FreqArray[0] - 48) * 1000 + (FreqArray[1] - 48) * 100 + (FreqArray[2] - 48) * 10 + (FreqArray[3] - 48))
#define HERequestNum ((HEArray[0] - 48) * 100 + (HEArray[1] - 48) * 10 + (HEArray[3] - 48))
#define LERequestNum ((LEArray[0] - 48) * 100 + (LEArray[1] - 48) * 10 + (LEArray[3] - 48))
#define INRequestNum ((INArray[0] - 48) * 1000 + (INArray[1] - 48) * 100 + (INArray[2] - 48) * 10 + (INArray[3] - 48))
// OTHER DEFENITION
#define FIRST_POSITION 8
#define SECOND_POSITION 98
#define ROW_PROFILE 265
#define PIXELS_HIGHLIGHT 8

#define MIN_DELAY 60
#define I2C1_DEVICE_ADDRESS 0x50 /* A0 = A1 = A2 = 0 */
#define SYNC_DELAY 50

extern bool StateParamsError;
extern bool StateLoadError;
extern bool TimersEnabled;
extern uint32_t pulse_count;
extern uint32_t counted_freq;
extern uint32_t last_counted_freq;
extern bool IsExternalSource;

// Вспомогательные
void UpdateExternalSource();
void SetExternalSource();
void UnsetExternalSource();
void counted_freq_to_array();
void LoadParametersFromProfile();
void CoreInit(
    I2C_HandleTypeDef *phi2c2,
    TIM_HandleTypeDef *phtim1,
    TIM_HandleTypeDef *phtim2,
    TIM_HandleTypeDef *phtim4);
bool IsChanged();

// Проверка на ошибки

void CheckParameters();

// Работа с памятью

void LoadParametersFromEEPROM();
void LoadProfilesFromEEPROM();
void EraseProfile();

// Работа с таймерами

void SetStateParameters();
void SetHZ(void);
void SetHE(void);
void SetINandLE(void);
void StopTimers();
void LaunchTimers();
void SetTimers();
void TryToSetStateParams();

// Отрисовка на экране
void UpdateScreenAfterUp();
void UpdateScreenAfterDown();
void UpdateScreenPlaceNumber();
void UpdateScreenAfterRight();
void PrintScreen();
void UpdateHZScreen();
// Обработка нажатий кнопок
void HandleButtonLeft(void);
void HandleButtonUp(void);
void HandleButtonDown(void);
void HandleButtonRight(void);
void HandleButtonIncrease(void);
void HandleButtonDecrease(void);
void HandleButtonSave(void);
void HandleButtonErase(void);
void HandleButtonHalfIN(void);
#endif