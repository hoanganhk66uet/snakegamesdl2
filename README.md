# snakegamesdl2
<h1>Dự án game môn Lập trình nâng cao</h1>
  <ul>
    <li>Tên sinh viên: Phạm Hoàng Anh
    <li>MSV: 21020278</li>
    <li>Lớp: K66CC</li></li>
  </ul>  
<h2>Cài đặt và chạy chương trình:</h2> 
  <ul>
    <li ">Dùng lệnh git clone để lưu thư mục về máy.</li>
    <li>Mở file Project snake_sdl.sln bằng Visual Studio.</li>
    <li>Run & Play</li>
  </ul>

<h2>Mô tả trò chơi</h2>  
  <ul>
    <li>Là game cổ điển đã xuất hiện từ rất lâu và từng được rất nhiều game thủ chơi trên những điện thoai nokia đời cũ. </li>
    <li>Thay vì chơi trên điện thoại thì game chơi trên máy tình và sử dụng các nút trên bàn phím (lên, xuống, trái, phải,space,esc) để xử lí rắn di chuyển và chuyển trạng thái.</li>
    <li>Người chơi sẽ di chuyển rắn và ăn quả để tăng kích thước.Trò chơi sẽ kết thúc khi đầu rắn chạm vào thân rắn hoặc người chơi thoát khỏi game khi đang chơi . </li>
  </ul>

<h2>Mô tả các chức năng đã cài đặt</h2>  
     <ul>
    <li>Hiển thị, xử lí các trạng thái của game như bắt đầu game, rắn ăn quả, dừng game, game kết thúc.</li>
    <li>Khi hiện cửa số game nhấn chuột vào "Play" để chơi,có hiệu ứng đổi màu khi di chuột vào "play".</li>
    <li>Tăng kích thước rắn khi ăn quả. </li>
    <li>Rắn có khả năng xuyên tường.</li>
    <li>Thân rắn sẽ đổi màu liên tục trong vòng 2s sau khi ăn quả.</li>
    <li>Đầu rắn quay theo hướng di chuyển của rắn.</li>
    <li>Game sẽ có nhạc trong quá trình trò chơi,khi ăn và khi kết thúc trò chơi.</li>
    <li>Khi rắn đang di chuyển nhấn "esc" hoặc phím "space" để dừng, sau đó nhấn "space" để tiếp tục hoặc "esc" để trở về màn hình lúc bắt đầu.</li>
    <li>Khi kết thúc game(đầu rắn chạm thân) dùng chuột bấm vào "yes" hoặc bấm phím "space" để chơi lại hoặc nhấn chuột vào "no" hoặc phím "esc" để trở về màn hình bắt đầu.</li>
    </ul>

<h2>Các kỹ thuật lập trình được sử dụng trong chương trình</h2>  
     <ul>                                                                                                                                          
    <li>Sử dụng kiểu cấu trúc "struct" cho game.</li>
    <li>Thân rắn dùng sử cấu trúc "vector" để thêm linh hoạt cho code khi làm dài rắn.</li>
    <li>Sử dụng con trỏ iterator để vòng lặp đơn giản, nhanh hơn .</li>
    <li>Phân loại chức năng rõ ràng(mỗi chức năng 1 hàm).</li>
    <li>Sử dụng đồ họa từ SDL như SDL2_image,SDL2_ttf,SDL2_mixture để tăng sự sinh động cho game.</li>
    </ul>

<h2>Kết luận, hướng phát triển và các điều tâm đắc rút ra được sau khi hoàn thiện chương trình</h2>  
    <ul>                                        
    <li>Game khá hoàn thiện chức năng cơ bản,tự đánh giá 7.5-8 đ. </li>
    <li>Game e cần bổ sung phần điểm số, đồng thời tạo thêm nhiều bản đồ hơn cho game,có thể tích hợp chế độ nhiều người chơi.</li>
    <li>Sau khi làm game e học được cách xây dựng 1 project nhỏ,tăng kĩ năng sử dụng con trỏ, vector,và quan trọng hơn nhất là cách tổ chức xử lí chức năng riêng bằng hàm.</li>
    </ul>


  
      
      
  
