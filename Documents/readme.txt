1 - TIM1_CH2 - PA9 - Выход базового таймера, от которого остальные триггерятся
2 - TIM2_CH1 - PA0 - Выход импульса HE
3 - TIM2_CH2 - PA1 - Синхроимпульс к  импульсу HE
2 - TIM4_CH1 - PB6 - Выход импульса LE
3 - TIM4_CH2 - PB7 - Синхроимпульс к  импульсу LE

// Обновление экрана при необходимости
    if (LastState != State || LastPlaceNumber != PlaceNumber || ValueChanged || Profile != LastProfile)
    {
      if (Profile != LastProfile)
      {
        SetParametersFromProfile(Profile);
        // Сохранение текущего профиля в EEPROM
        EEPROM_Write(&eeprom, 64, &Profile, 1, 15);
      }

      // Логика обработки изменения значений по кнопкам
      if (ValueChanged)
      {
        switch (State)
        {
        case HZ:
          if (FreqArray[4 - 1 - PlaceNumber] + ChangeDirection >= 48 &&
              FreqArray[4 - 1 - PlaceNumber] + ChangeDirection <= 57)
          {
            FreqArray[4 - 1 - PlaceNumber] += ChangeDirection;
          }
          break;
        case HE:
          if (HEArray[(4 - 1 - PlaceNumber) <= 2 ? (4 - 2 - PlaceNumber) : (4 - 1 - PlaceNumber)] + ChangeDirection >= 48 &&
              HEArray[(4 - 1 - PlaceNumber) <= 2 ? (4 - 2 - PlaceNumber) : (4 - 1 - PlaceNumber)] + ChangeDirection <= 57)
          {
            HEArray[(4 - 1 - PlaceNumber) <= 2 ? (4 - 2 - PlaceNumber) : (4 - 1 - PlaceNumber)] += ChangeDirection;
          }
          break;
        case LE:
          if (LEArray[(4 - 1 - PlaceNumber) <= 2 ? (4 - 2 - PlaceNumber) : (4 - 1 - PlaceNumber)] + ChangeDirection >= 48 &&
              LEArray[(4 - 1 - PlaceNumber) <= 2 ? (4 - 2 - PlaceNumber) : (4 - 1 - PlaceNumber)] + ChangeDirection <= 57)
          {
            LEArray[(4 - 1 - PlaceNumber) <= 2 ? (4 - 2 - PlaceNumber) : (4 - 1 - PlaceNumber)] += ChangeDirection;
          }
          break;
        case IN:
          if (INArray[3 - 1 - PlaceNumber] + ChangeDirection >= 48 &&
              INArray[3 - 1 - PlaceNumber] + ChangeDirection <= 57)
          {
            INArray[3 - 1 - PlaceNumber] += ChangeDirection;
          }
          break;
        }

        CheckParameters();
        if (!StateParamsError)
        {
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
          if (PulseEnabled)
          {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);
          }
          SetStateParameters();
        }
        else
        {
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
        }
      }

      UpdateScreenAfterUp(State, LastState, Profile, PlaceNumber,
                   LastPlaceNumber, FreqArray, HEArray, LEArray, INArray);

      // Индикация изменений
      if (IsChanged())
      {
        ST7789_DrawRect(FIRST_POSITION + 17 + 50 * Profile, 180,
                        FIRST_POSITION + 21 + 50 * Profile, 184,
                        CURRENT_STATE_COLOR);
      }
      else
      {
        ST7789_DrawRect(FIRST_POSITION + 17 + 50 * Profile, 180,
                        FIRST_POSITION + 21 + 50 * Profile, 184,
                        WHITE);
      }

      LastState = State;
      LastPlaceNumber = PlaceNumber;
      LastProfile = Profile;
      ValueChanged = false;
    }
  }
  PulseEnabled = !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
    if (PulseEnabled != LastPulseEnabled && !StateParamsError && !StateLoadError)
    {
      if (PulseEnabled)
      {
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
        HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
        HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);
      }
      else
      {
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
        __HAL_TIM_SET_COUNTER(&htim1, 0);
        __HAL_TIM_SET_COUNTER(&htim2, 0);
        __HAL_TIM_SET_COUNTER(&htim4, 0);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
      }
      LastPulseEnabled = PulseEnabled;
    }

    void UpdateScreenAfterUp()
{

    CurrentStateColor = StateParamsError ? ERROR_DEFAULT_FONT_COLOR : CURRENT_STATE_COLOR;

    switch (State)
    {
    case HZ:
        ST7789_PrintStr(FIRST_POSITION, 2, "HZ", 2, CurrentStateColor, BACKGROUND_COLOR, 0);
        ST7789_PrintCustomStr(SECOND_POSITION, 2, FreqArray, 4,
                              4 - 1 - PlaceNumber, CurrentStateColor,
                              CURRENT_STATE_COLOR, BACKGROUND_COLOR, 0);
        if (LastState != State)
        {
            ST7789_PrintStr(FIRST_POSITION + 8, ROW_PROFILE, "1", 1,
                            (Profile == P1) ? CURRENT_STATE_COLOR : DefaultFontColor, BACKGROUND_COLOR, 0);
            ST7789_PrintStr(FIRST_POSITION + 63, ROW_PROFILE, "2", 1,
                            (Profile == P2) ? CURRENT_STATE_COLOR : DefaultFontColor, BACKGROUND_COLOR, 0);
            ST7789_PrintStr(FIRST_POSITION + 129, ROW_PROFILE, "3", 1,
                            (Profile == P3) ? CURRENT_STATE_COLOR : DefaultFontColor, BACKGROUND_COLOR, 0);
            ST7789_PrintStr(FIRST_POSITION + 191, ROW_PROFILE, "4", 1,
                            (Profile == P4) ? CURRENT_STATE_COLOR : DefaultFontColor, BACKGROUND_COLOR, 0);
        }
        break;

    case HE:
        ST7789_PrintStr(FIRST_POSITION, 57, "HE", 2, CurrentStateColor, BACKGROUND_COLOR, 0);
        ST7789_PrintCustomStr(SECOND_POSITION, 57, HEArray, 4,
                              (4 - 1 - PlaceNumber) <= 2 ? (4 - 2 - PlaceNumber) : (4 - 1 - PlaceNumber),
                              CurrentStateColor, CURRENT_STATE_COLOR, BACKGROUND_COLOR, 0);
        if (LastState != State)
        {
            ST7789_PrintStr(FIRST_POSITION, 2, "HZ", 2, DefaultFontColor, BACKGROUND_COLOR, 0);
            ST7789_PrintStr(SECOND_POSITION, 2, FreqArray, 4, DefaultFontColor, BACKGROUND_COLOR, 0);
        }
        break;

    case LE:
        ST7789_PrintStr(FIRST_POSITION, 112, "LE", 2, CurrentStateColor, BACKGROUND_COLOR, 0);
        ST7789_PrintCustomStr(SECOND_POSITION, 112, LEArray, 4,
                              (4 - 1 - PlaceNumber) <= 2 ? (4 - 2 - PlaceNumber) : (4 - 1 - PlaceNumber),
                              CurrentStateColor, CURRENT_STATE_COLOR, BACKGROUND_COLOR, 0);
        if (LastState != State)
        {
            ST7789_PrintStr(FIRST_POSITION, 167, "HE", 2, DefaultFontColor, BACKGROUND_COLOR, 0);
            ST7789_PrintStr(SECOND_POSITION, 167, HEArray, 4, DefaultFontColor, BACKGROUND_COLOR, 0);
        }
        break;

    case IN:
        ST7789_PrintStr(FIRST_POSITION, 167, "IN", 2, CurrentStateColor, BACKGROUND_COLOR, 0);
        ST7789_PrintCustomStr(SECOND_POSITION, 167, INArray, 3,
                              3 - 1 - PlaceNumber, CurrentStateColor,
                              CURRENT_STATE_COLOR, BACKGROUND_COLOR, 0);
        if (LastState != State)
        {
            ST7789_PrintStr(FIRST_POSITION, 112, "LE", 2, DefaultFontColor, BACKGROUND_COLOR, 0);
            ST7789_PrintStr(SECOND_POSITION, 112, LEArray, 4, DefaultFontColor, BACKGROUND_COLOR, 0);
        }
        break;

    case PROFILE:
        ST7789_PrintStr(FIRST_POSITION + 8, ROW_PROFILE, "1", 1,
                        (Profile == P1) ? CURRENT_STATE_COLOR : CurrentStateColor, BACKGROUND_COLOR, 0);
        ST7789_PrintStr(FIRST_POSITION + 63, ROW_PROFILE, "2", 1,
                        (Profile == P2) ? CURRENT_STATE_COLOR : CurrentStateColor, BACKGROUND_COLOR, 0);
        ST7789_PrintStr(FIRST_POSITION + 129, ROW_PROFILE, "3", 1,
                        (Profile == P3) ? CURRENT_STATE_COLOR : CurrentStateColor, BACKGROUND_COLOR, 0);
        ST7789_PrintStr(FIRST_POSITION + 191, ROW_PROFILE, "4", 1,
                        (Profile == P4) ? CURRENT_STATE_COLOR : CurrentStateColor, BACKGROUND_COLOR, 0);
        if (LastState != State)
        {
            ST7789_PrintStr(FIRST_POSITION, 167, "IN", 2, DefaultFontColor, BACKGROUND_COLOR, 0);
            ST7789_PrintStr(SECOND_POSITION, 167, INArray, 3, DefaultFontColor, BACKGROUND_COLOR, 0);
        }
        break;
    }
}
