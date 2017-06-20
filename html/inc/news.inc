<?php
// This file is part of BOINC.
// http://boinc.berkeley.edu
// Copyright (C) 2008 University of California
//
// BOINC is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// BOINC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with BOINC.  If not, see <http://www.gnu.org/licenses/>.

function news_item($date, $title, $post) {
    echo "<p>";
    if ($title) {
        echo "<span class=\"lead\">$title</span><br>\n";
    }
    $d = time_str($date);
    $options = new output_options();
    $options->htmlitems = false;
    if (is_string($post)) {
        $forum_link = false;
        $text = output_transform($post, $options);
    } else {
        $forum_link = true;
        $text = output_transform($post->content, $options);
    }
    echo "
        $text
        <br>
        <span class=\"small\">$d
    ";
    if ($forum_link && !DISABLE_FORUMS) {
        echo "
            &middot; <a href=\"".url_base()."forum_thread.php?id=$post->thread\"> ".tra("Discuss")."</a>
        ";
    }
    echo "</span>
        <br clear=all>
        <hr>
    ";
}

function news_forum() {
    if (defined("NEWS_FORUM_NAME")) {
        $forum_name = NEWS_FORUM_NAME;
    } else {
        $forum_name = "News";
    }
    return BoincForum::lookup("parent_type=0 and title = '$forum_name'");
}

function is_news_forum($forum) {
    if (defined("NEWS_FORUM_NAME")) {
        $forum_name = NEWS_FORUM_NAME;
    } else {
        $forum_name = "News";
    }
    return $forum->parent_type == 0 && ($forum->title === $forum_name);
}

function show_news($start, $count) {
    $forum = news_forum();
    if (!$forum) {
        echo "
            No news forum.  Run html/ops/create_forums.php.
        ";
        return;
    }
    
    if (is_array($start)) {
        echo "You have not changed show_news() to use 0 instead of
            deprecated \$project_news as the first param.";
        return;
    }
    
    $lim = "";
    if ($start) {
        if ($count) {
            $lim = "limit $start, $count";
        } else {
            $lim = "limit $start, 99999999";
        }
    } else if ($count) {
        $lim = "limit $count";
    }
    $threads = BoincThread::enum("forum = $forum->id and hidden=0 order by id desc $lim");
    news_item(mktime(0, 0, 0, 4, 13, 2017), 'Начало', 'Был развернут боинк сервер на одном сервере. Тестируется работа.');
    news_item(mktime(0, 0, 0, 4, 28, 2017), 'Вынос БД', 'Была вынесенна базза на отдельный сервер. Проверяется производительность.');
    news_item(mktime(0, 0, 0, 5, 14, 2017), 'Разворачивание масштабируемой концигурации', 'Был развернут еще один боинк сервер на отдельном сервере и балансировщик нагрузки на отдельном. Тестируется работа.');
    news_item(mktime(0, 0, 0, 5, 26, 2017), 'Подключения Memcached как хранилище для сессий', 'Пользователь разлогиневался при перебрасывании на другой сервер. Для предотвращения столь печальной ситуации было решено сделать общее хранилище сессий посредством memcached.');
    news_item(mktime(0, 0, 0, 5, 30, 2017), 'GlusterFS', 'Была установлена GlusterFS для решения проблем с тем что пользователь не видет свой аватар на другом сервере..');
    news_item(mktime(0, 0, 0, 6, 3, 2017), 'Нагрузочное тестирование', 'Проведено нагрузочное тестирование, конфигурация справилась с нагрузкой.');
	
    foreach ($threads as $thread) {
        $posts = BoincPost::enum("thread=$thread->id order by id limit 1");
        $post = $posts[0];
        if (strstr($post->content, $thread->title) == $post->content) {
            $title = null;
        } else {
            $title = $thread->title;
        }
        news_item($thread->create_time, $title, $post);
    }

    if ($count) {
        echo "<a href=".url_base()."old_news.php>... more</a>";
    }
    echo "
        <br><br><small>
    ";
    printf(tra("News is available as an %sRSS feed%s"),
        "<a href=rss_main.php>",
        " &nbsp; <img src=\"img/rss_icon.gif\" alt=\"RSS\"></a>"
    );
    echo "
        </small>
    ";
}

$cvs_version_tracker[]="\$Id$";  //Generated automatically - do not edit
?>
