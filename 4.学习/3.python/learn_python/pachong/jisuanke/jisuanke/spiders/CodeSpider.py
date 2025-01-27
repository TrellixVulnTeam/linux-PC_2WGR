# -*- coding: utf-8 -*-
import scrapy
from scrapy import Request
from selenium import webdriver


class CodespiderSpider(scrapy.Spider):
    name = 'CodeSpider'
    allowed_domains = ['jisuanke.com']
    start_urls = ['https://www.jisuanke.com/course/786']
    cookie_file = './cookies_file'
    data_path = "./data/"


    def __init__(self):
        driver = webdriver.Chrome()
        driver.set_page_load_timeout(10)
        driver.get("http://www.jisuanke.com")
        with open(self.cookie_file,"r") as cookies:
            for each in cookies:
                name, value, domain = each.strip().split("\t")
                cookie = {
                    "name" : name,
                    "value" : value,
                    "domain" : domain
                }
                driver.add_cookie(cookie)
        self.driver = driver


    def parse(self, response):
        a_list = response.xpath("//a")
        href_list = []
        for a_item in a_list:
            title = a_item.xpath("./span[@class='lesson-icon-challenge']")
            if len(title) == 0:
                continue
            href_list.append(a_item.xpath("./@href")[0].extract())

        ind = 1
        for url in href_list:
            yield Request(url = "https:" + url, meta = {"index" : ind}, callback = self.parse2)
            ind += 1


    def parse2(self, response):
        tr_list = response.xpath("//div[@class='jsk-modal-bd']/table/tbody/tr")
        for tr in tr_list:
            tds = tr.xpath("./td[@class='jsk-text-success']")
            if len(tds) == 0:
                continue
            yield Request(url = tr.xpath(".//a/@href")[0].extract(), meta = response.meta, callback = self.save_code)
            break



    def save_code(self, response):
        file_name = self.data_path + str(response.meta["index"]) + ".cpp"
        code = response.xpath("//pre/text()")[0].extract()
        with open(file_name, "w") as file:
            file.write(code.encode("utf8"))
