import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Locale;
public class DateFormatSample02 {
  public static void main(String[] args) {
    // ロケールの設定（和暦用のロケール）
    Locale locale = new Locale("ja","JP","JP");
    // カレンダーオブジェクトの生成
    Calendar cal = Calendar.getInstance();
    
    // 和暦のロケールを指定して年をフォーマットする
    SimpleDateFormat sdf = new SimpleDateFormat("GGGGy年",locale);
    String dateStr = sdf.format(cal.getTime());
    
    // フォーマットされた日付の表示
    System.out.println(dateStr);
    // 年の短縮名を表示
    sdf.applyPattern("Gy");
    dateStr = sdf.format(cal.getTime());
    System.out.println(dateStr);
    
    // 100年前の和暦を表示してみる
    sdf.applyPattern("GGGGy年");
    cal.add(Calendar.YEAR, -100);
    dateStr = sdf.format(cal.getTime());
    System.out.println(dateStr);
  }
}