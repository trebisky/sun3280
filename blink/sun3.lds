SECTIONS
{
  . = 0x004000;
  .text :
  {
    *(.text)
    _etext = .;
    __etext = .;
  }
}
