#include "../STM8S_StdPeriph_Driver/inc/stm8s.h"


uint16_t test(void)
{

    uint8_t  val = 0x00, val_comp = 0x00;
    uint32_t add = 0x00;
		uint16_t data = 0x00;

    /* Define FLASH programming time */
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

    /* Unlock Data memory */
    FLASH_Unlock(FLASH_MEMTYPE_DATA);

    /* Read a byte at a specified address */
    add = 0x40A5;
    val = FLASH_ReadByte(add);

    /* Program complement value (of previous read byte) at previous address + 1 */
    val_comp = (uint8_t)(~val);
    FLASH_ProgramByte((add + 1), val_comp);

    /* Check program action */
    val = FLASH_ReadByte((add + 1));
    if (val != val_comp) {
        /* Error */
        data |= 0xFF00;
    }

    /* Erase byte at a specified address & address + 1 */
    FLASH_EraseByte(add);
    FLASH_EraseByte((add + 1));
    /* Erase action */
    val      = FLASH_ReadByte(add);
    val_comp = FLASH_ReadByte((add + 1));
    if ((val != 0x00) & (val_comp != 0x00)) {
        /* Error */
        data |= 0x00FF;
    }
		return data;
}
