package cz.cvut.fel.pjv;

import java.math.BigDecimal;
import java.math.MathContext;
import java.math.RoundingMode;
import java.util.Scanner;

public class Lab01 {
    public void homework() {
        System.out.println("Vyber operaci (1-soucet, 2-rozdil, 3-soucin, 4-podil):");
        Scanner sc = new Scanner(System.in);
        int scitanec = 0;
        scitanec = sc.nextInt();
        if (scitanec > 4 || scitanec < 1)
        {
            System.out.println("Chybna volba!");
            System.exit(100);
        }
        switch (scitanec)
        {
            case 1:
                System.out.println("Zadej scitanec: ");
                break;
            case 2:
                System.out.println("Zadej mensenec: ");
                break;
            case 3:
                System.out.println("Zadej cinitel: ");
                break;
            case 4:
                System.out.println("Zadej delenec: ");
                break;
        }
        float first = sc.nextFloat();
        switch (scitanec)
        {
            case 1:
                System.out.println("Zadej scitanec: ");
                break;
            case 2:
                System.out.println("Zadej mensitel: ");
                break;
            case 3:
                System.out.println("Zadej cinitel: ");
                break;
            case 4:
                System.out.println("Zadej delitel: ");
                break;
        }
        float second = sc.nextFloat();
        if (second == 0)
        {
            System.out.println("Pokus o deleni nulou!");
            System.exit(100);
        }
        System.out.println("Zadej pocet desetinnych mist: ");
        int comma = sc.nextInt();
        if (comma < 0)
        {
            System.out.println("Chyba - musi byt zadane kladne cislo!");
            System.exit(100);
        }
        MathContext mc = new MathContext(comma);
        BigDecimal res = new BigDecimal(first, mc);
        BigDecimal sec = new BigDecimal(second, mc);
        switch (scitanec)
        {
            case 1:
                System.out.printf("%." + Integer.toString(comma) + "f + %." + Integer.toString(comma) + "f = %." + Integer.toString(comma) + "f\n", res, sec, res.add(sec));
                break;
            case 2:
                System.out.printf("%." + Integer.toString(comma) + "f - %." + Integer.toString(comma) + "f = %." + Integer.toString(comma) + "f\n", res, sec, res.subtract(sec));
                break;
            case 3:
                System.out.printf("%." + Integer.toString(comma) + "f * %." + Integer.toString(comma) + "f = %." + Integer.toString(comma) + "f\n", res, sec, res.multiply(sec));
                break;
            case 4:
                System.out.printf("%." + Integer.toString(comma) + "f / %." + Integer.toString(comma) + "f = %." + Integer.toString(comma) + "f\n", res, sec, res.divide(sec, comma, RoundingMode.HALF_UP));
                break;
        }
    }
}
