# -*- coding: utf-8 -*-
"""ergasia3.2.ipynb

Automatically generated by Colaboratory.

Original file is located at
    https://colab.research.google.com/drive/1KNpAt0MpJ3wucv3h2jB5rZtKzHUeTC5w

# Imports
"""

!pip install idx2numpy
import keras
import numpy as np
import pandas as pd
from keras import layers, optimizers, losses, metrics
import idx2numpy
import matplotlib.pyplot as plt

from keras.layers import Input,Conv2D,MaxPooling2D,UpSampling2D
from keras.models import Model
from keras.optimizers import RMSprop

from sklearn.model_selection import train_test_split

from keras.layers import Input, Dense, Conv2D, MaxPooling2D, UpSampling2D, Flatten, Reshape
from keras.models import Model

"""# Data Read and Check"""

# set data and query paths
data_path = '/content/input.dat'
query_path = '/content/query.dat'

# load data and query from paths
data = idx2numpy.convert_from_file(data_path)
query = idx2numpy.convert_from_file(query_path)

# Display the reshaped image
plt.imshow(data[120], cmap='gray')
plt.title("Example Image")
plt.show()

# shape of data set
print("Data set shape: ",data.shape)

# shape of query set
print("Query set shape: ",query.shape)

"""# Data Preproccessing

Convert from 28x28 to 28x28x1
"""

# underscored data and query hold the postproccessed data, and query sets
_data = data.reshape(-1, 28,28, 1)
_query = query.reshape(-1, 28,28, 1)

# shape of data set
print("Data set shape: ",_data.shape)

# shape of query set
print("Query set shape: ",_query.shape)

"""Check new format of data"""

data[0]

_data[0]

"""Change data type to float32"""

# change the data type of _data and _query to float32
_data = _data.astype('float32')
_query = _query.astype('float32')

_data.dtype, _query.dtype

"""Rescale values from [0, 255] to [0.0, 1.0]"""

np.max(_data), np.max(_query)

_data = _data / np.max(_data)
_query = _query / np.max(_query)

np.max(_data), np.max(_query)

"""Split the data into train, valid, ground"""

train_X,valid_X,train_ground,valid_ground = train_test_split(_data,
                                                             _data,
                                                             test_size=0.181,
                                                             random_state=333)

"""## Convolutional Autoencoder

Set hyperparameters and create input layer
"""

# hyperparameters
batch_size = 128 # 128, 256, 512
epochs = 5

# create the input layer
inChannel = 1
x, y = 28, 28
input_img = Input(shape = (x, y, inChannel))

"""Define the network"""

def autoencoder(input_img):

    f1,f2,f3=4,8,16

    #encoder
    #input = 28 x 28 x 1 (wide and thin)
    conv1 = Conv2D(f1, (3, 3), activation='relu', padding='same')(input_img) #28 x 28 x f1
    pool1 = MaxPooling2D(pool_size=(2, 2))(conv1) #14 x 14 x f1
    conv2 = Conv2D(f2, (3, 3), activation='relu', padding='same')(pool1) #14 x 14 x f2
    pool2 = MaxPooling2D(pool_size=(2, 2))(conv2) #7 x 7 x f2
    conv3 = Conv2D(f3, (3, 3), activation='relu', padding='same')(pool2) #7 x 7 x f3 (small and thick)

    #decoder
    conv4 = Conv2D(f3, (3, 3), activation='relu', padding='same')(conv3) #7 x 7 x f3
    up1 = UpSampling2D((2,2))(conv4) # 14 x 14 x f3
    conv5 = Conv2D(f2, (3, 3), activation='relu', padding='same')(up1) # 14 x 14 x f2
    up2 = UpSampling2D((2,2))(conv5) # 28 x 28 x f2
    decoded = Conv2D(1, (3, 3), activation='sigmoid', padding='same')(up2) # 28 x 28 x 1
    return decoded

"""Compile model"""

autoencoder = Model(input_img, autoencoder(input_img))
autoencoder.compile(loss='mean_squared_error', optimizer = RMSprop())

autoencoder.summary()

autoencoder_train = autoencoder.fit(train_X, train_ground, batch_size=batch_size,epochs=epochs,verbose=1,validation_data=(valid_X, valid_ground))

loss = autoencoder_train.history['loss']
val_loss = autoencoder_train.history['val_loss']
epochs = range(epochs)
plt.figure()
plt.plot(epochs, loss, 'bo', label='Training loss')
plt.plot(epochs, val_loss, 'b', label='Validation loss')
plt.title('Training and validation loss')
plt.legend()
plt.show()

pred = autoencoder.predict(_query)

pred.shape

plt.figure(figsize=(20, 4))
print("Test Images")
for i in range(10):
    plt.subplot(2, 10, i+1)
    plt.imshow(_query[i, ..., 0], cmap='gray')
plt.show()
plt.figure(figsize=(20, 4))
print("Reconstruction of Test Images")
for i in range(10):
    plt.subplot(2, 10, i+1)
    plt.imshow(pred[i, ..., 0], cmap='gray')
plt.show()

encoder_model = Model(inputs=autoencoder.input, outputs=autoencoder.get_layer('conv2d_131').output)

bottleneck_output = encoder_model.predict(_query)

bottleneck_output.shape

bottleneck_output[0]

"""## Convolutional Autoencoder Bottleneck

Set hyperparameters and create input layer
"""

# hyperparameters
batch_size = 128 # 128, 256, 512
epochs = 2

# create the input layer
inChannel = 1
x, y = 28, 28
input_img = Input(shape = (x, y, inChannel))

"""Define the network"""

def autoencoder(input_img):

    f1, f2, f3 = 4, 8, 16
    latent_dim=10

    # Encoder
    conv1 = Conv2D(f1, (3, 3), activation='relu', padding='same')(input_img)  # 28 x 28 x f1
    pool1 = MaxPooling2D(pool_size=(2, 2))(conv1)  # 14 x 14 x f1
    conv2 = Conv2D(f2, (3, 3), activation='relu', padding='same')(pool1)  # 14 x 14 x f2
    pool2 = MaxPooling2D(pool_size=(2, 2))(conv2)  # 7 x 7 x f2
    conv3 = Conv2D(f3, (3, 3), activation='relu', padding='same')(pool2)  # 7 x 7 x f3 (small and thick)

    # Flatten before the bottleneck layer
    flatten = Flatten()(conv3)

    # Bottleneck layer with latent_dim neurons
    bottleneck_output = Dense(latent_dim, activation='sigmoid')(flatten)

    # Decoder
    reshaped = Reshape((1, 1, latent_dim))(bottleneck_output)
    up1 = UpSampling2D((7, 7))(reshaped)  # Adjust the spatial dimensions
    conv4 = Conv2D(f3, (3, 3), activation='relu', padding='same')(up1)  # 7 x 7 x f3
    up2 = UpSampling2D((2, 2))(conv4)  # 14 x 14 x f3
    conv5 = Conv2D(f2, (3, 3), activation='relu', padding='same')(up2)  # 14 x 14 x f2
    up3 = UpSampling2D((2, 2))(conv5)  # 28 x 28 x f2
    decoded = Conv2D(1, (3, 3), activation='sigmoid', padding='same')(up3)  # 28 x 28 x 1

    return decoded

"""Compile model"""

autoencoder = Model(input_img, autoencoder(input_img))
autoencoder.compile(loss='mean_squared_error', optimizer = RMSprop())

autoencoder.summary()

autoencoder_train = autoencoder.fit(train_X, train_ground, batch_size=batch_size,epochs=epochs,verbose=1,validation_data=(valid_X, valid_ground))

loss = autoencoder_train.history['loss']
val_loss = autoencoder_train.history['val_loss']
epochs = range(epochs)
plt.figure()
plt.plot(epochs, loss, 'bo', label='Training loss')
plt.plot(epochs, val_loss, 'b', label='Validation loss')
plt.title('Training and validation loss')
plt.legend()
plt.show()

pred = autoencoder.predict(_query)

pred.shape

plt.figure(figsize=(20, 4))
print("Test Images")
for i in range(10):
    plt.subplot(2, 10, i+1)
    plt.imshow(_query[i, ..., 0], cmap='gray')
plt.show()
plt.figure(figsize=(20, 4))
print("Reconstruction of Test Images")
for i in range(10):
    plt.subplot(2, 10, i+1)
    plt.imshow(pred[i, ..., 0], cmap='gray')
plt.show()

encoder_model = Model(inputs=autoencoder.input, outputs=autoencoder.get_layer('dense_18').output)

bottleneck_output = encoder_model.predict(_query)

bottleneck_output.shape

bottleneck_output[3]



"""# bottleneck 2"""

# hyperparameters
batch_size = 128 # 128, 256, 512
epochs = 5

# create the input layer
inChannel = 1
x, y = 28, 28
input_img = Input(shape = (x, y, inChannel))

input_img = Input(shape = (x, y, inChannel))

x = Conv2D(16, (3, 3), activation='relu', padding='same')(input_img)
x = MaxPooling2D((2, 2), padding='same')(x)
x = Conv2D(8, (3, 3), activation='relu', padding='same')(x)
x = MaxPooling2D((2, 2), padding='same')(x)
x = Conv2D(4, (3, 3), activation='relu', padding='same')(x)
x = MaxPooling2D((2, 2), padding='same')(x)

# Flatten to prepare for the dense layer
x = Flatten()(x)

# Latent space
latent = Dense(10, activation='relu')(x)

# Decoder
x = Dense(4 * 4 * 4, activation='relu')(latent)
x = Reshape((4, 4, 4))(x)
x = Conv2D(4, (3, 3), activation='relu', padding='same')(x)
x = UpSampling2D((2, 2))(x)
x = Conv2D(8, (3, 3), activation='relu', padding='same')(x)
x = UpSampling2D((2, 2))(x)
x = Conv2D(16, (3, 3), activation='relu')(x)
x = UpSampling2D((2, 2))(x)

# Output layer
decoded = Conv2D(1, (3, 3), activation='sigmoid', padding='same')(x)

# Autoencoder model
autoencoder = Model(input_img, decoded)
autoencoder.compile(loss='mean_squared_error', optimizer = RMSprop())

autoencoder.summary()

autoencoder_train = autoencoder.fit(train_X, train_ground, batch_size=batch_size,epochs=epochs,verbose=1,validation_data=(valid_X, valid_ground))

loss = autoencoder_train.history['loss']
val_loss = autoencoder_train.history['val_loss']
epochs = range(epochs)
plt.figure()
plt.plot(epochs, loss, 'bo', label='Training loss')
plt.plot(epochs, val_loss, 'b', label='Validation loss')
plt.title('Training and validation loss')
plt.legend()
plt.show()

pred = autoencoder.predict(_query)

pred.shape

plt.figure(figsize=(20, 4))
print("Test Images")
for i in range(10):
    plt.subplot(2, 10, i+1)
    plt.imshow(_query[i, ..., 0], cmap='gray')
plt.show()
plt.figure(figsize=(20, 4))
print("Reconstruction of Test Images")
for i in range(10):
    plt.subplot(2, 10, i+1)
    plt.imshow(pred[i, ..., 0], cmap='gray')
plt.show()

encoder_model = Model(inputs=autoencoder.input, outputs=autoencoder.get_layer('conv2d_131').output)

bottleneck_output = encoder_model.predict(_query)

bottleneck_output.shape

bottleneck_output[0]

