package com.projecttango.experiments.nativepointcloud;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.YuvImage;
import android.hardware.Camera;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.List;
import java.util.Random;

/**
 * Created by lisa on 16.05.15.
 */
public class CameraView extends SurfaceView implements SurfaceHolder.Callback {
    private Camera camera;
    private Bitmap bmp;

    public CameraView( Context context ) {
        super( context );
        // We're implementing the Callback interface and want to get notified
        // about certain surface events.
        getHolder().addCallback( this );
        // We're changing the surface to a PUSH surface, meaning we're receiving
        // all buffer data from another component - the camera, in this case.
        getHolder().setType( SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS );
    }

    public Bitmap getGreen(){
        return this.bmp;
    }

    public void surfaceCreated( SurfaceHolder holder ) {
        // Once the surface is created, simply open a handle to the camera hardware.
        camera = Camera.open();
        /*camera.setPreviewCallback(new Camera.PreviewCallback() {
                                      public void onPreviewFrame(byte[] data, Camera camera) {
                                          try {
                                              // pWidth and pHeight define the size of the preview Frame
                                              ByteArrayOutputStream out = new ByteArrayOutputStream();
                                              Camera.Parameters mParameters = camera.getParameters();
                                              Camera.Size mSize = mParameters.getPreviewSize();
                                              int mWidth = mSize.width;
                                              int mHeight = mSize.height;
                                              // Alter the second parameter of this to the actual format you are receiving
                                              YuvImage yuv = new YuvImage(data, ImageFormat.NV21, mWidth, mHeight, null);
                                              yuv.compressToJpeg(new Rect(0, 0, mWidth, mHeight), 50, out);
                                              byte[] bytes = out.toByteArray();
                                              Bitmap bmp = BitmapFactory.decodeByteArray(bytes, 0, bytes.length);

                                              //TODO save to sd card
                                              //saveImage(bmp);

                                              int pixel = bmp.getPixel(0, 0);
                                              int redValue = Color.red(pixel);
                                              int blueValue = Color.blue(pixel);
                                              int greenValue = Color.green(pixel);
                                              //Log.d("OMFG",""+redValue);

                                          } catch (Exception e) {
                                              e.printStackTrace();
                                          }
                                      }

                                      private void saveImage(Bitmap finalBitmap) {

                                          String root = Environment.getExternalStorageDirectory().toString();
                                          File myDir = new File(root + "/saved_images");
                                          myDir.mkdirs();
                                          /*Random generator = new Random();
                                          int n = 10000;
                                          n = generator.nextInt(n);
                                          String fname = "Image-"+ n +".jpg";*/
                                          /*String fname = "img.jpg";
                                          Log.d("OMFG","");
                                          File file = new File(myDir, fname);
                                          if (file.exists()) file.delete();
                                          try {
                                              FileOutputStream out = new FileOutputStream(file);
                                              finalBitmap.compress(Bitmap.CompressFormat.JPEG, 90, out);
                                              out.flush();
                                              out.close();

                                          } catch (Exception e) {
                                              e.printStackTrace();
                                          }
                                      }
                                  }
        );*/

    }

    public void surfaceChanged( SurfaceHolder holder, int format, int width, int height ) {
        // This method is called when the surface changes, e.g. when it's size is set.
        // We use the opportunity to initialize the camera preview display dimensions.
        Camera.Parameters parameters = camera.getParameters();
        List<Camera.Size> previewSizes = parameters.getSupportedPreviewSizes();

        // You need to choose the most appropriate previewSize for your app
        Camera.Size previewSize = previewSizes.get(0);

                parameters.setPreviewSize(previewSize.width, previewSize.height);
        camera.setParameters(parameters);

        // We also assign the preview display to this surface...
        try {
            camera.setPreviewDisplay( holder );
        } catch( IOException e ) {
            e.printStackTrace();
        }
        // ...and start previewing. From now on, the camera keeps pushing preview
        // images to the surface.
        camera.startPreview();
    }

    public void surfaceDestroyed( SurfaceHolder holder ) {
        // Once the surface gets destroyed, we stop the preview mode and release
        // the whole camera since we no longer need it.
        camera.stopPreview();
        camera.release();
        camera = null;
    }
}