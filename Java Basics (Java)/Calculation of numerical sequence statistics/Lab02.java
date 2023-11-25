/*
 * File name: Lab06.java
 * Date:      2014/08/26 21:39
 * Author:    @author
 */

package cz.cvut.fel.pjv;

public class Lab02 {

   public void homework() {
      TextIO text = new TextIO();
      String s = "s";
      int i = 0;
      int line = 0;
      float prumer = 0;
      float odch = 0;
      float[] nums = new float[10];
      while ((s = text.getLine()) != "")
      {
         line++;
         if (TextIO.isFloat(s) || TextIO.isDouble(s) || TextIO.isInteger(s))
         {
            nums[i] = Float.parseFloat(s);
            prumer += nums[i];

            i++;
            if (i == 10)
            {
               prumer /= i;
               for (int j = 0; j < i; j++)
               {
                  odch += Math.pow((nums[j] - prumer), 2);
               }
               odch = (float)Math.sqrt(odch / i);
               System.out.printf("%2d %.3f %.3f\n", i, prumer, odch);
               i = 0;
               odch = 0;
               prumer = 0;
            }

         }
         else
         {
            System.err.println("A number has not been parsed from line " + line);
         }
      }
      System.err.println("End of input detected!");
      if (i > 1)
      {
         prumer /= i;
         for (int j = 0; j < i; j++)
         {
            odch += Math.pow((nums[j] - prumer), 2);
         }
         odch = (float)Math.sqrt(odch / i);
         System.out.printf("%2d %.3f %.3f\n", i, prumer, odch);
      }
   }
}
