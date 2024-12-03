#include "Header/definitions.h"

internal_function uptr
StringLength(char *String)
{
    uptr Result = 0;

    while(*String++ != '\0')
    {
        Result++;
    }

    return Result;
}

internal_function uptr
CopyCharacterToBuffer(char *Buffer, uptr BufferSize, int Character)
{
    uptr Result = 0;

    if(BufferSize < sizeof(char))
    {
        Assert(0);
        return Result;
    }

    Buffer[0] = (char)Character;

    Result = 1;
    return Result;
}

internal_function uptr
CopyStringToBuffer(char *Buffer, uptr BufferSize, char *String, uptr StringSize, uptr Length)
{
    uptr Result = 0;

    uptr MaxLength = StringSize;
    if(Length > MaxLength)
    {
        Assert(0);
        Length = MaxLength;
    }

    for(uptr Index = 0; Index < Length; Index++)
    {
        Result += CopyCharacterToBuffer(Buffer + Result, BufferSize - Result, String[Index]);
    }

    return Result;
}

internal_function uptr
CopySigned32ToBuffer(char *Buffer, uptr BufferSize, s32 Value, int Base, char *Representation)
{
    uptr Result = 0;

    if(!Value)
    {
        Result = CopyCharacterToBuffer(Buffer, BufferSize, '0');
        return Result;
    }

    bool IsNegative = Value < 0;
    if(IsNegative)
    {
        Value = -Value;
    }

    char WorkBuffer[32] = {0};
    int WorkBufferIndex = ArrayCount(WorkBuffer) - 2;
    for(; Value; Value /= Base)
    {
        WorkBuffer[WorkBufferIndex--] = Representation[Value % Base];
    }

    if(IsNegative)
    {
        WorkBuffer[WorkBufferIndex] = '-';
    }
    else
    {
        WorkBufferIndex++;
    }

    Result = CopyStringToBuffer(Buffer, BufferSize, WorkBuffer + WorkBufferIndex, StringLength(WorkBuffer + WorkBufferIndex), StringLength(WorkBuffer + WorkBufferIndex));
    return Result;
}

internal_function uptr
CopyUnsigned32ToBuffer(char *Buffer, uptr BufferSize, u32 Value, int Base, char *Representation)
{
    uptr Result = 0;

    if(!Value)
    {
        Result = CopyCharacterToBuffer(Buffer, BufferSize, '0');
        return Result;
    }
    
    char WorkBuffer[32] = {0};
    int WorkBufferIndex = ArrayCount(WorkBuffer) - 2;
    for(; Value; Value /= Base)
    {
        WorkBuffer[WorkBufferIndex--] = Representation[Value % Base];
    }

    WorkBufferIndex++;
    Result = CopyStringToBuffer(Buffer, BufferSize, WorkBuffer + WorkBufferIndex, StringLength(WorkBuffer + WorkBufferIndex), StringLength(WorkBuffer + WorkBufferIndex));

    return Result;
}

internal_function uptr
CopySigned64ToBuffer(char *Buffer, uptr BufferSize, s64 Value, int Base, char *Representation)
{
    uptr Result = 0;

    if(!Value)
    {
        Result = CopyCharacterToBuffer(Buffer, BufferSize, '0');
        return Result;
    }

    bool IsNegative = Value < 0;
    if(IsNegative)
    {
        Value = -Value;
    }

    char WorkBuffer[64] = {0};
    int WorkBufferIndex = ArrayCount(WorkBuffer) - 2;
    for(; Value; Value /= Base)
    {
        WorkBuffer[WorkBufferIndex--] = Representation[Value % Base];
    }

    if(IsNegative)
    {
        WorkBuffer[WorkBufferIndex] = '-';
    }
    else
    {
        WorkBufferIndex++;
    }

    Result = CopyStringToBuffer(Buffer, BufferSize, WorkBuffer + WorkBufferIndex, StringLength(WorkBuffer + WorkBufferIndex), StringLength(WorkBuffer + WorkBufferIndex));
    return Result;
}

internal_function uptr
CopyUnsigned64ToBuffer(char *Buffer, uptr BufferSize, u64 Value, int Base, char *Representation)
{
    uptr Result = 0;

    if(!Value)
    {
        Result = CopyCharacterToBuffer(Buffer, BufferSize, '0');
        return Result;
    }
    
    char WorkBuffer[64] = {0};
    int WorkBufferIndex = ArrayCount(WorkBuffer) - 2;
    for(; Value; Value /= Base)
    {
        WorkBuffer[WorkBufferIndex--] = Representation[Value % Base];
    }

    WorkBufferIndex++;
    Result = CopyStringToBuffer(Buffer, BufferSize, WorkBuffer + WorkBufferIndex, StringLength(WorkBuffer + WorkBufferIndex), StringLength(WorkBuffer + WorkBufferIndex));
    
    return Result;
}

internal_function uptr
CopyFloat64ToBuffer(char *Buffer, uptr BufferSize, f64 Value, int Precision)
{
    uptr Result = 0;

    char *Decimal = "0123456789";
    int IntegerPart = (int)Value;
    Result = CopySigned32ToBuffer(Buffer, BufferSize, IntegerPart, 10, Decimal);
    Result += CopyCharacterToBuffer(Buffer + Result, BufferSize - Result, '.');

    if(Value < 0)
    {
        Value = -Value;
        IntegerPart = -IntegerPart;
    }

    Value -= IntegerPart;
    
    for(int Index = 0; Index < Precision; Index++)
    {
        Value *= 10;
        int Digit = (int)Value;
        Result += CopySigned32ToBuffer(Buffer + Result, BufferSize - Result, Digit, 10, Decimal);
        Value -= Digit;
    }

    return Result;
}

internal_function bool
IsSpace(int Character)
{
    bool Result = 0;

    char c = (char)Character;
    if((c == ' ') || (c == '\f') || (c == '\n') || (c == '\r') || (c == '\t') || (c == '\v'))
    {
        Result = 1;
    }

    return Result;
}

internal_function bool
IsDigit(int Character)
{
    bool Result = 0;

    char c = (char)Character;
    if((c >= '0') && (c <= '9'))
    {
        Result = 1;
    }

    return Result;
}

internal_function bool
ReadFloat642(f64 *Value, char *s)
{
    bool Result = 0;

    bool IsNegative = 0;
    if(*s == '-')
    {
        IsNegative = 1;
        s++;
    }

    if(!IsDigit(*s))
    {
        Log("Float value should start with a digit or - sign\n");
        return Result;
    }

    int IntegerCount = 0;
    int DecimalCount = 0;
    int DotCount = 0;
    int IntegerPart = 0;
    int DecimalPart = 0;
    for(char *c = s; (*c != '\0') && (IsDigit(*c) || (*c == '.')); c++)
    {
        if(*s == '.')
        {
            DotCount++;
            continue;
        }

        if(!DotCount)
        {
            IntegerCount++;
        }
        else
        {
            DecimalCount++;
        }        

        if(!DotCount)
        {
            IntegerPart = (IntegerPart * 10) + (*c - '0');
        }
        else
        {
            DecimalPart = (DecimalPart * 10) + (*c - '0');
        }
    }
    
    *Value = (f64)DecimalPart;
    for(int Index = 0; Index < DecimalCount; Index++)
    {
        *Value /= 10.0;
    }
    
    *Value += (f64)IntegerPart;
    *Value = (IsNegative) ? (-(*Value)) : (*Value);
    Result = (IntegerCount >= 1) && (!DotCount || (DotCount == 1)) && (DecimalCount >= 0);
    return Result;
}

internal_function bool
ParseFloat64(char *String)
{
    bool Result = 0;

    if(*String == '-')
    {
        String++;
    }

    if(!IsDigit(*String))
    {
        Log("Float value doesn't start with a digit: %c\n", *String);
        return Result;
    }

    int IntegerCount = 0;
    int DotCount = 0;
    int DecimalCount = 0;
    int IntegerPart = 0;
    int DecimalPart = 0;
    for(char *c = String; IsDigit(*c); c++)
    {
        if(*c == '.')
        {
            DotCount++;
            continue;
        }

        if(!DotCount)
        {
            IntegerCount++;
        }
        else
        {
            DecimalCount++;
        }        
    }

    Result = ((IntegerCount >= 1) && (!DotCount || (DotCount == 1)) && (DecimalCount >= 0)) ? (1) : (0);
    return Result;
}

internal_function f64
ReadFloat64(char *String)
{
    f64 Result = 0;

    bool IsNegative = 0;
    if(*String == '-')
    {
        IsNegative = 1;
        String++;
    }
    
    int IntegerCount = 0;
    int DotCount = 0;
    int DecimalCount = 0;
    int IntegerPart = 0;
    int DecimalPart = 0;
    for(char *c = String; IsDigit(*c); c++)
    {
        if(*c == '.')
        {
            DotCount++;
            continue;
        }

        if(!DotCount)
        {
            IntegerCount++;
            IntegerPart = (IntegerPart * 10) + (*c - '0');
        }
        else
        {
            DecimalCount++;
            DecimalPart = (DecimalPart * 10) + (*c - '0');
        }        
    }

    Result = DecimalPart;
    for(int Index = 0; Index < DecimalCount; Index++)
    {
        Result /= 10;
    }

    Result += IntegerPart;
    Result = (IsNegative) ? (-Result) : (Result);

    return Result;
}