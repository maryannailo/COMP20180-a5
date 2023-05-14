void dev_null_init(void);
void dev_zero_init(void);
void devconsoleinit(void);

void devinit(void)
{
  dev_null_init();
  dev_zero_init();
  devconsoleinit();
}
