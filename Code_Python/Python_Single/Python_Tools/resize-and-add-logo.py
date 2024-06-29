# resize-and-add-logo.py
# This script helps resize all images in the given directory to 
# fit in a 1920x1280 screen(especially the width), and add catlogo.png to the lower-right corner.
import os
from PIL import Image, ImageFile
ImageFile.LOAD_TRUNCATED_IMAGES = True # tolerate large image file

FIT_WIDTH = 1920
LOG_WIDTH = 180
LOGO_FILENAME = 'Python_Tools\\catlogo.png' # in current working directory
GIVEN_DIR = 'C:\\Users\\21839\\Pictures\\image'

logoIm = Image.open(LOGO_FILENAME)
logoWidth, logoHeight = logoIm.size # (808,768)
logoHeight, logoWidth = int((LOG_WIDTH / logoWidth) * logoHeight), LOG_WIDTH
logoIm = logoIm.resize((logoWidth, logoHeight))

os.makedirs('withLogo', exist_ok = True) # create a directory to save changed images
for filename in os.listdir(GIVEN_DIR):
    if not (filename.endswith('.png') or filename.endswith('.jpg')):
        continue # skip non-image files
    im = Image.open(os.path.join(GIVEN_DIR, filename))
    width, height = im.size
    if width > FIT_WIDTH:
        height = int((FIT_WIDTH / width) * height)
        width = FIT_WIDTH
    print('Resizing {}...'.format(filename)) 
    im = im.resize((width, height)) # resize the image
    print('Adding logo to {}...'.format(filename))
    im.paste(logoIm, (width - logoWidth, height - logoHeight), logoIm) # add the logo
    im.save(os.path.join('withLogo', filename)) # save changes


 
