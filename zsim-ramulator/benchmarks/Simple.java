import java.util.ArrayList;
import java.util.Random;
 

class Simple{
    private static Random random = new Random();
    private static final ArrayList<Integer> list = new ArrayList<>();
    public static void main(String args[]){
        for (int i = 0; i < 500; i++) {
            list.add(random.nextInt());
		try{
        		Thread.sleep(10);
		} catch (Exception e){

		}
        }
	System.gc();
for (int i = 0; i < 500; i++) {
            list.add(random.nextInt());
		try{
        		Thread.sleep(10);
		} catch (Exception e){

		}
        }
	System.gc();


    }
}
