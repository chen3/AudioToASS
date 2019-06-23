package cn.mx404.audiotoass

import kotlinx.coroutines.coroutineScope
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import java.io.Closeable
import java.lang.RuntimeException
import java.nio.file.Files
import java.nio.file.Path
import kotlin.concurrent.thread

class LocalServer : Closeable {

    companion object {
        private const val clientExeFileName = "client";
        private val logger: Logger = LoggerFactory.getLogger(LocalServer::class.java);
    }

    private enum class Status {
        NotStart, Preparing, Started, Closed
    }
    @Volatile
    private var status: Status = Status.NotStart;

    private var clientProcess: Process? = null;

    private val lock: Any = Object();

    suspend fun startLocalClient() {
        if (status != Status.NotStart) {
            throw IllegalStateException("current local server status:${status.name}, require ${Status.NotStart.name}");
        }
        synchronized(lock) {
            if (status != Status.NotStart) {
                throw IllegalStateException("current local server status:${status.name}, require ${Status.NotStart.name}");
            }
            status = Status.Preparing;
        }
        status = Status.Preparing;
        if (!Files.exists(Path.of(clientExeFileName))) {
            throw RuntimeException("$clientExeFileName not exists.");
        }
        return coroutineScope {

            thread(start = true) {
                clientProcess = Runtime.getRuntime().exec(clientExeFileName);
            }
        }
    }

    override fun close() {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }
}