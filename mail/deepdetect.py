from email import message_from_file
from email.header import decode_header
from random import sample
from string import hexdigits
from zipfile import ZipFile, is_zipfile
from datetime import datetime
import re
import base64

""" Deep detect mail attachment.
Get mail info from received eml file
Detect attachment file type,and save attachment
"""


def process_main_text(main_text):
    # print(main_text)
    pass


def process_attachment(content, filename, directory='./'):
    """If file already exist,below code will overwrite it.
    Set directory to different path is best choice.
    You'd better do NOT use default value!"""
    with open(directory + filename, 'wb') as fp:
        fp.write(content)


def parse_payload(payload):
    # walk just like its name,will return all part of attachment and main text
    for part in payload.walk():
        main_text_payload = part.get_payload()

        if isinstance(main_text_payload, str):
            # content type will show main content or attachment type
            filename = part.get_filename()
            if filename is not None:
                # attachment has file name
                utf_filename = decode_header(filename)[0][0]
                if isinstance(utf_filename, bytes):
                    utf_filename = utf_filename.decode(guess_mail_charset(filename))
                if part.get('Content-Transfer-Encoding') == 'base64':
                    main_text = base64.standard_b64decode(main_text_payload)
                process_attachment(main_text, utf_filename)
            else:
                # main text or html
                pattern = r'(charset=\")([a-z0-9\-]{3,10})\"'
                regexp = re.compile(pattern)
                m = regexp.search(part.as_string(True))
                if m is not None:
                    charset = m.group(2)
                    # print('\n\n-------%s----------' % charset)
                else:
                    charset = 'utf-8'

                if part.get('Content-Transfer-Encoding') == 'base64':
                    main_text = base64.standard_b64decode(main_text_payload)

                if part.get('Content-Transfer-Encoding') == 'quoted-printable':
                    main_text = main_text_payload

                if isinstance(main_text, bytes):
                    try:
                        main_text = main_text.decode(charset)
                    except UnicodeDecodeError:
                        print("UnicodeDecodeError:", end='')
                # process main text or html
                process_main_text(main_text)


def parse_mail_payload(file):
    with open(file) as fp:
        print('\n:filename %s' % file)
        mail_content, mail_subject = load_mail_from_file(fp)

    # get_payload() will return all attachment and main text as Message Object
    # in a list
    msg_list = mail_content.get_payload()
    # It seemed that every email are all multipart
    # print(mail_content.is_multipart())
    for msg_part in msg_list:
        if isinstance(msg_part, list):
            for i in msg_part:
                parse_payload(i)
        else:
            parse_payload(msg_part)


def guess_mail_charset(sample_string):
    """Use re to detect mail charset . sample_string is original text from mail
    and should be contain charset information just like '=?GB2312?B?' """
    pattern = r'=\?([A-Za-z0-9\-]{3,10})\?[A-Z]{1}\?'
    m = re.search(pattern, sample_string)
    if m is not None:
        return m.group(1)
    return 'utf-8'


def load_mail_from_file(mail_file):
    """ Download mail and return mail content.Return subject is to used to generate email file name
    'subject' is dict key in email head ,'from' ,'to' can be acquire too"""

    message_class = message_from_file(mail_file)
    mail_subject = message_class.get('subject')
    # charset = message_class.get_charsets(failobj='utf-8')
    # get_charsets is not accuracy
    if mail_subject != '':
        # if you are sure that all mails' charset are utf-8
        # guess_mail_charset() func can be omit
        # charset = guess_mail_charset(mail_subject)
        charset = guess_mail_charset(mail_subject)
        utf_subject = decode_header(mail_subject)[0][0]
    else:
        utf_subject = None
        charset = 'utf-8'

    if isinstance(utf_subject, bytes):
        try:
            utf_subject = utf_subject.decode(charset)
        except UnicodeDecodeError:
            # It is hard to detect charset so generate random string
            # as mail file name
            utf_subject = None
        finally:
            if utf_subject is None:
                utf_subject = ''.join(sample(hexdigits, 8))

    return message_class, utf_subject


def unzip_attachment(attachment_file, unzip_file_path):
    __ZIP_PASSWD__ = '123456'
    if not is_zipfile(attachment_file):

        return

    zip_file = ZipFile(attachment_file)
    for pack_file in zip_file.namelist():
        zip_file.extract(pack_file, path=unzip_file_path, pwd=__ZIP_PASSWD__)


def filter_time(eml_date_string, delta_days):
    """eml_date_string can be obtained email by get('Date') function
    delta_days is int value means if email received date before delta_days
    will return false
    """
    now_dt = datetime.now()
    fm_time = str()
    time_list = eml_date_string.split()
    for i in range(0, 5):
        fm_time += time_list[i]

    eml_dt = datetime.strptime(fm_time, '%a,%d%b%Y%H:%M:%S')
    return (now_dt - eml_dt).days < delta_days

if __name__ == '__main__':
    file_list = ['fw_attachment_pt.eml',
                 'fw_attachment_x.eml',
                 'fw_no_attachment.eml',
                 'photo_mail.eml',
                 'plain.eml'
                 ]
    for f in file_list:
        parse_mail_payload(f)
