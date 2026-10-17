#include "GUI.h"

#include <stdio.h>
#include <time.h>

#include "Lunar.h"
#include "fonts.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define GFX_printf_styled(gfx, fg, bg, font, ...) \
    GFX_setTextColor(gfx, fg, bg);                \
    GFX_setFont(gfx, font);                       \
    GFX_printf(gfx, __VA_ARGS__);

// height to use larger layout
#define large_layout(data) ((data)->height >= 400)

typedef struct {
    uint8_t month;
    uint8_t day;
    char name[10];  // 3x3+1
} Festival;

static const Festival festivals[] = {
    {1, 1, "元旦節"},  {2, 14, "情人節"}, {3, 8, "婦女節"},  {3, 12, "植樹節"},  {4, 1, "愚人節"},
    {5, 1, "勞動節"},  {5, 4, "青年節"},  {6, 1, "兒童節"},  {7, 1,},   {8, 1,},
    {9, 10, "教師節"}, {10, 1, "國慶節"}, {11, 1, "萬聖節"}, {12, 24, "平安夜"}, {12, 25, "聖誕節"},
} ;

static const Festival festivals_lunar[] = {
    {1, 1, "春節"},    {1, 15, "元宵節"}, {2, 2, "龍抬頭"},  {5, 5, "端午節"},  {7, 7, "七夕節"}, {7, 15, "中元節"},
    {8, 15, "中秋節"}, {9, 9, "重陽節"},  {10, 1, "寒衣節"}, {12, 8, "腊八節"}, {12, 30, "除夕"},
} ;

// 放假和調休數據，每年更新
#define HOLIDAY_YEAR 2026
static const uint16_t holidays[] = {
    0x0101, 0x0211, 0x0212, 0x0213,  // 1月1日元旦, 2月17-19日農曆年初一/二/三
    0x0403, 0x0404,                      // 4月3日耶穌受難節, 4月4日翌日
    0x0406, 0x0407,                      // 4月6日清明節次日, 4月7日復活節星期一翌日
    0x0501, 0x0519,                      // 5月1日勞動節, 5月25日佛誕翌日
    0x0613,                              // 6月19日端午節
    0x0701,                              // 7月1日香港特別行政區成立紀念日
    0x091A, 0x091B,                      // 9月26日中秋節翌日, 9月27日(實際安排而定)
    0x0A01, 0x0A13,                      // 10月1日國慶日, 10月19日重陽節次日
    0x0C19, 0x0C1A                       // 12月25日聖誕節, 12月26日聖誕節後第一個周日
} ;

static bool GetHoliday(uint8_t mon, uint8_t day, bool* work) {
    for (uint8_t i = 0; i < ARRAY_SIZE(holidays); i++) {
        if (((holidays[i] >> 8) & 0xF) == mon && (holidays[i] & 0xFF) == day) {
            *work = ((holidays[i] >> 12) & 0xF) > 0;
            return true;
        }
    }
    return false;
}

static bool GetFestival(uint16_t year, uint8_t mon, uint8_t day, uint8_t week, struct Lunar_Date* Lunar,
                        char* festival) {
    // 农历节日
    for (uint8_t i = 0; i < ARRAY_SIZE(festivals_lunar); i++) {
        if (Lunar->Month == festivals_lunar[i].month && Lunar->Date == festivals_lunar[i].day) {
            strcpy(festival, festivals_lunar[i].name);
            return true;
        }
    }

    // 除夕：春节前一天（12/29 或 12/30），12/30 已在上面判断
    if (Lunar->Month == 12 && Lunar->Date == 29) {
        struct Lunar_Date nextLunar;
        struct devtm tm = {year, mon, day, 0, 0, 0, week};
        transformTime(transformTimeStruct(&tm) + 86400, &tm);
        LUNAR_SolarToLunar(&nextLunar, tm.tm_year + YEAR0, tm.tm_mon + 1, tm.tm_mday);
        if (nextLunar.Month == 1 && nextLunar.Date == 1) {
            strcpy(festival, "除夕");
            return true;
        }
    }
    // 母亲节: 五月第二个星期日
    if (mon == 5 && week == 0 && day >= 8 && day <= 14) {
        strcpy(festival, "母亲节");
        return true;
    }
    // 父亲节: 六月第三个星期日
    if (mon == 6 && week == 0 && day >= 15 && day <= 21) {
        strcpy(festival, "父亲节");
        return true;
    }
    // 感恩节：十一月第四个星期四
    if (mon == 11 && week == 4 && day >= 22 && day <= 28) {
        strcpy(festival, "感恩节");
        return true;
    }
    // 公历节日
    for (uint8_t i = 0; i < ARRAY_SIZE(festivals); i++) {
        if (mon == festivals[i].month && day == festivals[i].day) {
            strcpy(festival, festivals[i].name);
            return true;
        }
    }
    // 二十四节气
    uint8_t JQdate;
    if (GetJieQi(year, mon, day, &JQdate) && JQdate == day) {
        uint8_t JQ = (mon - 1) * 2;
        if (day >= 15) JQ++;
        strcpy(festival, JieQiStr[JQ]);
        if (JQ == 6)  // 清明
            strcat(festival, "节");
        return true;
    }

    return false;
}