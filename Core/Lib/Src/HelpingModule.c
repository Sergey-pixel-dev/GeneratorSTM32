#include "HelpingModule.h";
void NumberToArrayBuffer(__UINT16_TYPE__ n)
{
	buffer_len = 0;
	__UINT8_TYPE__ i = 0;

	__UINT16_TYPE__ buf_n = n;
	while (buf_n > 0)
	{
		buf_n /= 10;
		buffer_len += 1;
	}
	while (n > 0)
	{
		buffer[buffer_len - i - 1] = n % 10;
		n /= 10;
		i++;
	}
}
