void MODULE_NAME(void)
{
  volatile int i;
  i = 1; 
  i = i + 1;  /* squelch set-but-not-used compiler warning */
  return; 
}


