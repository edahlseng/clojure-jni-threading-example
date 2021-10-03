import com.nativeutils.NativeUtils;
import java.io.File;
import java.io.IOException;

public class Test {
    static {
        System.out.println("Loading C library from Java!");
        try {
            NativeUtils.loadLibraryFromJar("/Test.so");
        } catch (IOException e) {
            System.out.println("Error encountered while attempting to load library:");
            e.printStackTrace();
        }
    }

    public static native void test(TestFunction function);
}
