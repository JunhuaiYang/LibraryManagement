create view record_view as 
select record_15693.recordID, record_15693.isRenting, record_15693.cardID, user_15693.name, user_15693.telphone, 
    record_15693.booksID , books_15693.name, 
    books_15693.author, books_15693.publishing_house,  record_15693.lend_time, record_15693.return_time 
from books_15693 , record_15693, user_15693 
where record_15693.booksID = books_15693.booksID and record_15693.cardID = user_15693.cardID