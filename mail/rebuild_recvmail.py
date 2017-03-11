from imaplib import IMAP4_SSL, IMAP4
from email import message_from_string
from email.header import decode_header
from random import sample
from string import hexdigits
from re import search
from time import sleep
from os.path import isfile
from datetime import datetime
# below package should be install by manual
from configparser import ConfigParser


""" This programme is use for receive email from hotmail.com with IMAP Protocol
If used to receive 126.com 163.com will raise error at login step
This Programme is Written by james at 2017.01.19
You can use it and modify it freely BUT DO NOT REMOVE COPYRIGHT INFO
install pip3:
> sudo apt-get install python-pip
> sudo pip3 install ConfigParser
"""
# print more information
__DEBUG__ = True
# if true script will test mail file name,it's means more mail will
# be written down
__FILE_TEST__ = False

def guess_mail_charset(sample_string):
    """Use re to detect mail charset . sample_string is original text from mail
    and should be contain charset information just like '=?GB2312?B?' """
    pattern = r'=\?([A-Za-z0-9\-]{3,10})\?[A-Z]{1}\?'
    m = search(pattern, sample_string)
    if m is not None:
        return m.group(1)

    return 'utf-8'


def load_config_file(segment=None):
    """ If segment is None func will read 'global' segment and get 'enable'
    value.This value will tell func which segment parameter will be used.
    If segment is NOT None,func will use pointed segment parameter """
    config = ConfigParser()
    config.read('mail.ini')
    directory = config.get('global', 'dir')
    if directory is None:
        directory = './'
    if config.get('global', 'delete') == '1':
        is_del = True
    else:
        is_del = False
    if segment is None:
        segment = config.get('global', 'enable')

    server = config.get(segment, "server")
    port = config.get(segment, "port")
    user = config.get(segment, "user")
    passwd = config.get(segment, "passwd")

    return server, int(port), user, passwd, is_del, directory


def connect_server(server, port, user, passwd):
    """Connect IMAP server. Return server connection and mailbox_status"""
    server_connection = IMAP4_SSL(server, port)
    if server_connection is not None:
        try:
            mailbox_status = server_connection.login(user, passwd)
        except IMAP4.error as err:
            # The best way is store err message into log
            # but this exception always means passwd or user fail
            return None, None
    else:
        return None, None
    return server_connection, mailbox_status


def search_mailbox(server_connection, box='INBOX', flag='UNSEEN'):
    """Search mailbox. Default parameter will search INBOX which UNSEEN.
    Return mail_list and mailbox status,if success status is OK"""
    box_status = server_connection.select(box)
    _, mail_status = server_connection.search(None, flag)
    mail_list = mail_status[0].split()
    return box_status, mail_list


def windows_style(func):
    """Decorate for windows style"""
    def _windows(conn, mail):
        _content, _subject = func(conn, mail)
        _w_content = _content.replace('\n', '\r\n')
        return _w_content, _subject
    return _windows


@windows_style
def download_mails(server_connection, mail):
    """ Download mail and return mail content.Return subject is to used to generate email file name
    'subject' is dict key in email head ,'from' ,'to' can be acquire too"""
    # if connection unstable exception could be raised
    _, message_data = server_connection.fetch(mail, '(RFC822)')
    charset = 'utf-8'
    for response_part in message_data:
        if isinstance(response_part, tuple):
            # Get email.message.Message class instance
            # Test if email download complete
            # Standard is end with "--\n"
            email_content = response_part[1].decode()
            if not (email_content.endswith('--\r\n') or email_content.endswith('--\n')):
                print('Download Email Error')
                return None, None
            message_class = message_from_string(email_content)
            mail_subject = message_class.get('subject')
            if mail_subject != '':
                # if you are sure that all mails' charset are utf-8
                # guess_mail_charset() func can be omit
                charset = guess_mail_charset(mail_subject)
                utf_subject = decode_header(mail_subject)[0][0]
            else:
                utf_subject = None

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
            email_content = message_class.as_string()
            email_content = email_content.replace('\n','\r')
    return email_content, utf_subject


def get_email_filename(directory, email_subject):
    """From email subject generate file name.If file has been exits or create file fail
    the file name is 8 random hex digits string"""
    # Test if email file name is validate

    if email_subject is None:
        if __DEBUG__:
            print('Email subject is None')
        if __FILE_TEST__:
            return directory + ''.join(sample(hexdigits, 8)) + '.eml'
        else:
            return None

    # replace invalid char
    email_subject.replace('/', '_')
    filename = directory + email_subject + '.eml'
    # if file exits rename

    if isfile(filename):
        if __DEBUG__:
            print('Finde Same Email subject [ %s ]' % filename)
        if __FILE_TEST__:
            copys = ''.join(sample(hexdigits, 3))
            return directory + email_subject + '_' + copys + '.eml'
        else:
            return None
    
    return filename


def change_mail_status(server_connection, mail, status=r'(\Seen)'):

    _, response = server_connection.store(mail, '+FLAGS', status)
    return response


def main():
    process_count = 0
    server, port, user, passwd, is_del, directory = load_config_file('mike')
    print('Receive mail from [ %s ]' % user)

    conn, mail_status = connect_server(server, port, user, passwd)

    if conn is not None:
        print('%s ----> %s' % (mail_status[1][0].decode(), mail_status[0]))
    else:
        print('Login fail!!!')
        return

    box_status, mail_list = search_mailbox(conn)
    print('Searching INBOX  ----> %s' % box_status[0])
    total_mail_count = len(mail_list)

    print('Find UNSEEN Mail in INBOX %d ' % total_mail_count)
    if total_mail_count == 0:
        return

    if is_del:
        tick = 20
        print('!!!According to mail.ini email will be DELETE after download!!!\nIF NOT SURE Press Ctrl-c %02d'
              % tick, flush=True, end='')
        while tick >= 0:
            print('\b\b%02d' % tick, end='', flush=True)
            tick -= 1
            sleep(1)
    print('\nProcess ', end='', flush=True)
    # deal with mails
    try:
        for mail in mail_list:
            mail_content, mail_subject = download_mails(conn, mail)
            filename = get_email_filename(directory, mail_subject)
            if filename is not None:
                with open(filename, 'w') as fp:
                    fp.write(mail_content)
                change_mail_status(conn, mail)
            process_count += 1
            if is_del:
                change_mail_status(conn, mail, r'(\Deleted)')

            print('%03d of %03d' % (process_count, total_mail_count), end="", flush=True)
            print('\b\b\b\b\b\b\b\b\b\b', end='', flush=True)
        print('\n')
    finally:
        if is_del:
            conn.expunge()
        conn.close()
        conn.logout()


if __name__ == "__main__":
    try:

        while True:
            print("Begin Receive Mail at ", end="")
            print(datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
            main()
            print("Sleeping...")
            sleep(1*60)
    except KeyboardInterrupt as e:
        print("\b\bI was so tired!")
        exit()
