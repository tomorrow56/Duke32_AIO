/******************************************
* Duke32-AIO FW�������݃X�N���v�g�ɂ��� *
*                              2020/05/13 *
*              Masao Yamazaki(@tomorrow56)*
*******************************************/

<�t�H���_�̓��e>
readme.txt                  ���̃t�@�C��
esptool.exe                 �����݃\�t�g�{��
esp32_BTO_firmware_v1.bin   �f�t�H���gFW
Firmware_Backup_dd.cmd      FW�o�b�N�A�b�v�p�X�N���v�g
Firmware_Restore_dd.cmd     FW�������ݗp�X�N���v�g

<����m�F��>
Windows10 PC (FTDI�h���C�o������)
USB�|�[�g: USB2.0�𐄏�(3.0�ł͏������߂Ȃ��E���x���o�Ȃ��P�[�X���H�ɂ���)

<�����ݑO����>
1. �{�[�h��Micro USB�P�[�u��(�������ݑΉ�)�Őڑ�
2. �V���A���|�[�g�Ƃ��ĔF�������̂ŁA�f�o�C�X�}�l�W���[���Ń|�[�g�ԍ����m�F
3. Firmware_Restore_dd.cmd �̈ȉ��̕�����ڑ�����Ă���|�[�g�ԍ��ɏC��
 @echo off
 SET COM_PORT=COM8  <- ����

<�����ݎ菇>
1. ����t�H���_�Ɉȉ��̃t�@�C�����i�[����
  esptool.exe
  esp32_BTO_firmware_v1.bin
  Firmware_Restore_dd.cmd

2. PC�ƃ{�[�h��USB�P�[�u���Őڑ�����

3. Windows�̃G�N�X�v���[����1.�̃t�H���_���J��

4. Firmware_Restore_dd.cmd �� esp32_BTO_firmware_v1.bin ���h���b�O�h���b�v����

5. �R�}���h�v�����v�g���J�������݂��n�܂�̂ŁA�I������܂ő҂�
   (��30�b���x�ŏ������݂͊�������)

<�ǉ�: ��������ł���FW�̃o�b�N�A�b�v>
1. Firmware_Backup_dd.cmd �̈ȉ��̕�����ڑ�����Ă���|�[�g�ԍ��ɏC��
 @echo off
 SET COM_PORT=COM8  <- ����

2. ����t�H���_�Ɉȉ��̃t�@�C�����i�[����
  esptool.exe
  Firmware_Backup_dd.cmd

3. �o�b�N�A�b�v�������t�@�C�����̋�t�@�C�����쐬����
  - �e�L�X�g�G�f�B�^�Ńt�@�C����V�K�쐬���Ē��g����̂܂ܖ��O��K���ɂ���̂��ȒP
  - �t�@�C�����̊g���q��*.bin����������

4. PC�ƃ{�[�h��USB�P�[�u���Őڑ�����

5. Windows�̃G�N�X�v���[����1.�̃t�H���_���J��

4. Firmware_Backup_dd.cmd �� ��L�ō쐬������t�@�C�����h���b�O�h���b�v����
  - �Ԉ���ĕʃt�@�C�����h���b�O�h���b�v����Ɩ������ŏ㏑�������̂Œ���

5. �R�}���h�v�����v�g���J���o�b�N�A�b�v���n�܂�̂ŁA�I������܂ő҂�
   (��60�b���x�Ńo�b�N�A�b�v�͊�������)

6. �o�b�N�A�b�v�t�@�C���̃T�C�Y��"4096KB"�ł���΃o�b�N�A�b�v�͊���



