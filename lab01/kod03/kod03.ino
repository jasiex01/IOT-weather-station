//DO NOT COMPILE - FOR INFORMATION ONLY!
int main(void)
{
    init();

    initVariant();

#if defined(USBCON)
    USBDevice.attach();
#endif

    setup();

    for (;;)
    {
        loop();
        if (serialEventRun)
            serialEventRun();
    }

    return 0;
}