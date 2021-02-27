int isNumber(char* stringReg)
{
    int condition = 1;
    int i = 0;
    while(!(stringReg[i]=='\0'))
    {
        if(stringReg[i]=='-' || stringReg[i]=='+')
        {
            i++;
            continue;
        }
        if(!(isdigit(stringReg[i])))
        {
            condition = 0;
            return condition; //return 0; <----change to this 
        }
        i++;
    }
    return condition;
}